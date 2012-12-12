
#include <unordered_map>

#include "CompilerContext.h"

#include "ErrorFactory.h"
#include "SymbolTable.h"
#include "FunctionProcedureEntity.h"
#include "IdentifierDispenser.h"

#include "llvm/BasicBlock.h"
#include "llvm/Support/Dwarf.h"
#include "llvm/Analysis/DIBuilder.h"
#include "llvm/Analysis/DebugInfo.h"
#include "Utilities.h"

using namespace std;

#define SHA_MALICE 1179

namespace MAlice {
    
    CompilerContext::CompilerContext(std::string input)
    {        
        m_DebugBuilder = NULL;
        initialiseCompilerContext(input);
    }
    
    CompilerContext::CompilerContext(std::string input, std::string sourceFile, std::string dirFile)
    {
        initialiseCompilerContext(input);
        m_DebugBuilder = new llvm::DIBuilder(*m_module);
        sourceFile += ".alice";

        m_DebugBuilder->createCompileUnit(llvm::dwarf::DW_LANG_lo_user + SHA_MALICE,
                                          sourceFile, dirFile, "MAliceATRCompiler", false, "", 1);

        m_dbfile = m_DebugBuilder->createFile(sourceFile, dirFile);
    }

    CompilerContext::~CompilerContext()
    {
        for (list<SymbolTable*>::iterator it = m_symbolTables.begin(); it != m_symbolTables.end(); ++it)
        {
            SymbolTable *table = *it;

            if (table) {
                delete table, table = NULL;
            }
        }
        m_symbolTables.clear();
        
        // The entities for the function procedure scope stack are cleaned up in the symbol tables.
        while(!m_functionProcedureScopeStack.empty()) {
            m_functionProcedureScopeStack.pop();
        }
        
        if (m_lexer)
            delete m_lexer, m_lexer = NULL;
        
        if (m_parser)
            delete m_parser, m_parser = NULL;
        
        if (m_inputStream)
            delete m_inputStream, m_inputStream = NULL;
        
        if (m_tokenStream)
            delete m_tokenStream, m_tokenStream = NULL;
        
        if (m_irBuilder)
            delete m_irBuilder, m_irBuilder = NULL;
        
        if (m_identifierDispenser)
            delete m_identifierDispenser, m_identifierDispenser = NULL;
    }

    void CompilerContext::initialiseCompilerContext(std::string input)
    {
        m_input = input;
        withinExpressionTree = false;
        // Set the ANTLR structure defaults
        m_lexer = NULL;
        m_parser = NULL;
        m_inputStream = NULL;
        m_tokenStream = NULL;
        
        t_symbolTable = new SymbolTable();
        m_errorReporter = NULL;
        configureKeywords();
        #ifdef _WIN32
           temporarySymbolTableLock = CreateMutex(0, FALSE, 0);
        #else
           pthread_mutex_init (&temporarySymbolTableLock, NULL);;
        #endif
           //m_initBlock = new llvm::BasicBlock(llvm::getGlobalContext(), "
        m_irBuilder = new llvm::IRBuilder<>(llvm::getGlobalContext());
        m_module = new llvm::Module("root module", llvm::getGlobalContext());
        m_identifierDispenser = new IdentifierDispenser();
        m_dbgScope = std::vector<llvm::MDNode*>();
        m_symbolTables.push_back(new SymbolTable());
    }
    
    bool CompilerContext::lockTemporarySymbolTable()
    {
        #ifdef _WIN32
            return (WaitForSingleObject(temporarySymbolTableLock, INFINITE)==WAIT_FAILED?1:0);  
        #else
           return pthread_mutex_lock( &temporarySymbolTableLock );  
        #endif
    }

    bool CompilerContext::unlockTemporarySymbolTable()
    {
        #ifdef _WIN32
            return (ReleaseMutex(temporarySymbolTableLock)==0);
        #else
            return pthread_mutex_unlock( &temporarySymbolTableLock );
        #endif
    }

    SymbolTable* CompilerContext::getTemporarySymbolTable()
    {
      return t_symbolTable;
    }

    bool CompilerContext::addEntityInScope(std::string identifier, Entity *entity)
    {
        if (m_symbolTables.empty())
            return false;
        
        SymbolTable* & innermostSymbolTable = m_symbolTables.back();
        if (innermostSymbolTable->contains(identifier))
            return false;
        
        innermostSymbolTable->insert(identifier, entity);
        
        return true;
    }
    
    bool CompilerContext::isSymbolInScope(std::string identifier, Entity **outEntity)
    {
        if (m_symbolTables.empty())
            return false;
        
        for (list<SymbolTable*>::reverse_iterator iterator = m_symbolTables.rbegin(); iterator != m_symbolTables.rend(); iterator++) {
            SymbolTable *table = *iterator;
            Entity *entity = table->get(identifier);
            
            if (entity) {
                if (outEntity)
                    *outEntity = entity;
                
                return true;
            }
        }
        
        return false;
     }
    
    bool CompilerContext::isSymbolInCurrentScope(std::string identifier, Entity **outEntity)
    {
        if (m_symbolTables.empty())
            return false;
        
        SymbolTable *table = m_symbolTables.back();
        Entity *entity = table->get(identifier);
        if (entity != NULL && outEntity != NULL)
            *outEntity = entity;
        
        return entity != NULL;
    }

    void CompilerContext::enterDebugScope(ASTNode node)
    {
         if (m_DebugBuilder)
         {
             llvm::DIDescriptor newscope = m_DebugBuilder->createLexicalBlock(
                 this->getCurrentDBScope() == NULL?
                 llvm::DIDescriptor() :
                 llvm::DIDescriptor(this->getCurrentDBScope()),
                 *this->getDIFile(),
                 Utilities::getNodeLineNumber(node),
                 Utilities::getNodeColumnIndex(node));
                 m_dbgScope.push_back(newscope);

         }
    }

    void CompilerContext::enterScope()
    {
        m_symbolTables.push_back(new SymbolTable());
    }
    
    void CompilerContext::exitDebugScope(ASTNode node)
    {
        if (m_DebugBuilder)
        {
            m_dbgScope.pop_back();
        }
    }
    void CompilerContext::exitScope()
    {
        if (m_symbolTables.empty()) {
            ErrorReporter *errorReporter = getErrorReporter();
            
            if (errorReporter) {
                errorReporter->reportError(ErrorFactory::createInternalError("Trying to pop an empty Compiler Context scope stack."));
                return;
            }
        }
        
        SymbolTable* & lastSymbolTable = m_symbolTables.back();
        delete lastSymbolTable;
        
        m_symbolTables.pop_back();
    }

    llvm::MDNode* CompilerContext::getCurrentDBScope()
    {
        return m_dbgScope.empty()?NULL:m_dbgScope.back();
    }
    
    ErrorReporter *CompilerContext::getErrorReporter()
    {
        return m_errorReporter;
    }
    
    void CompilerContext::setErrorReporter(ErrorReporter *errorReporter)
    {
        if (m_errorReporter != NULL)
            m_errorReporter->setInput("");
        
        m_errorReporter = errorReporter;
        m_errorReporter->setInput(m_input);
    }
    
    void CompilerContext::configureKeywords()
    {
        m_keywords.insert("opened");
        m_keywords.insert("closed");
        m_keywords.insert("spider");
        m_keywords.insert("but");
        m_keywords.insert("The");
        m_keywords.insert("looking-glass");
        m_keywords.insert("room");
        m_keywords.insert("spoke");
        m_keywords.insert("was");
        m_keywords.insert("a");
        m_keywords.insert("became");
        m_keywords.insert("Alice");
        m_keywords.insert("found");
        m_keywords.insert("because");
        m_keywords.insert("said");
        m_keywords.insert("then");
        m_keywords.insert("and");
        m_keywords.insert("contained");
        m_keywords.insert("eventually");
        m_keywords.insert("enough");
        m_keywords.insert("times");
        m_keywords.insert("unsure");
        m_keywords.insert("which");
        m_keywords.insert("or");
        m_keywords.insert("so");
        m_keywords.insert("piece");
        m_keywords.insert("what");
        m_keywords.insert("of");
        m_keywords.insert("too");
        m_keywords.insert("maybe");
        m_keywords.insert("perhaps");
        m_keywords.insert("either");
        m_keywords.insert("had");
        m_keywords.insert("ate");
        m_keywords.insert("drank");
        m_keywords.insert("number");
        m_keywords.insert("letter");
        m_keywords.insert("sentence");
    }
    
    bool CompilerContext::isKeyword(std::string string)
    {
        return m_keywords.find(string) != m_keywords.end();
    }
    
    pMAliceLexer CompilerContext::getLexer()
    {
        return m_lexer;
    }
    
    void CompilerContext::setLexer(pMAliceLexer lexer)
    {
        m_lexer = lexer;
    }
    
    pMAliceParser CompilerContext::getParser()
    {
        return m_parser;
    }
    
    void CompilerContext::setParser(pMAliceParser parser)
    {
        m_parser = parser;
    }
    
    pANTLR3_INPUT_STREAM CompilerContext::getInputStream()
    {
        return m_inputStream;
    }
    
    void CompilerContext::setInputStream(pANTLR3_INPUT_STREAM inputStream)
    {
        m_inputStream = inputStream;
    }
    
    pANTLR3_COMMON_TOKEN_STREAM CompilerContext::getTokenStream()
    {
        return m_tokenStream;
    }
    
    void CompilerContext::setTokenStream(pANTLR3_COMMON_TOKEN_STREAM tokenStream)
    {
        m_tokenStream = tokenStream;
    }
    
    FunctionProcedureEntity *CompilerContext::getCurrentFunctionProcedureEntity()
    {
        if (m_functionProcedureScopeStack.empty())
            return NULL;
        
        return m_functionProcedureScopeStack.top();
    }
    
    void CompilerContext::pushFunctionProcedureEntity(FunctionProcedureEntity *entity)
    {
        m_functionProcedureScopeStack.push(entity);
    }
    
    void CompilerContext::popFunctionProcedureEntity()
    {
        if (m_functionProcedureScopeStack.empty())
            return;
        m_functionProcedureScopeStack.pop();
        
    }
    bool CompilerContext::withinExpression() {
        return withinExpressionTree;
    }

    void CompilerContext::beginExpression()
    {
        withinExpressionTree = true;
    }
    void CompilerContext::endExpression()
    {
        withinExpressionTree = false;
    }
    
    llvm::IRBuilder<> *CompilerContext::getIRBuilder()
    {
        return m_irBuilder;
    }
    
    llvm::Module *CompilerContext::getModule()
    {
        return m_module;
    }

    void CompilerContext::clearSemanticInformation() 
    {
        for (list<SymbolTable*>::iterator it = m_symbolTables.begin(); it != m_symbolTables.end(); ++it)
        {
            SymbolTable *table = *it;

            if (table) {
                delete table, table = NULL;
            }
        }
        
        m_symbolTables.clear();
        
        // Push back another global scope symboltable
        m_symbolTables.push_back(new SymbolTable());
        
        // The entities for the function procedure scope stack are cleaned up in the symbol tables.
        while(!m_functionProcedureScopeStack.empty()) {
            m_functionProcedureScopeStack.pop();
        }
    }

    llvm::DIFile *CompilerContext::getDIFile()
    {
        return &m_dbfile;
    }
    
    IdentifierDispenser *CompilerContext::getIdentifierDispenser()
    {
        return m_identifierDispenser;
    }
    
    void CompilerContext::setIdentifierDispenser(IdentifierDispenser *dispenser)
    {
        m_identifierDispenser = dispenser;
    }

    llvm::DIBuilder* CompilerContext::getDGBuilder()
    {
        return m_DebugBuilder;
    }
    
    llvm::Value *CompilerContext::ioFormatStringForExpressionType(Type type)
    {
        std::unordered_map<unsigned int, llvm::Value*>::iterator element = m_ioFormatStringMap.find(type.getPrimitiveType());
        if (element != m_ioFormatStringMap.end())
            return element->second;
        
        llvm::Value *value = NULL;
        
        switch(type.getPrimitiveType())
        {
            case PrimitiveTypeSentence:
                value = getIRBuilder()->CreateGlobalStringPtr("%s", "__io_string_format");
                break;
            case PrimitiveTypeNumber:
                value = getIRBuilder()->CreateGlobalStringPtr("%llu", "__io_number_format");
                break;
            case PrimitiveTypeBoolean:
                value = getIRBuilder()->CreateGlobalStringPtr("%d", "__io_bool_format");
                break;
            case PrimitiveTypeLetter:
                value = getIRBuilder()->CreateGlobalStringPtr("%c", "__io_char_format");
                break;
            default:
                return NULL;
                break;
        }
        
        m_ioFormatStringMap.insert(std::make_pair(type.getPrimitiveType(), value));
        
        return value;
    }

    llvm::Function *CompilerContext::prettyPrintBoolFunction()
    {
        std::vector<llvm::Type*> parameterList;
        parameterList.push_back(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeBoolean)));
        
        std::string functionName = "MAlice_PrettyPrintBool";
        llvm::Function *prettyPrintFunction = getModule()->getFunction(functionName);
        if (prettyPrintFunction)
            return prettyPrintFunction;
        
        llvm::FunctionType *functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()),
                                                                   parameterList,
                                                                   false);
        
        prettyPrintFunction = llvm::Function::Create(functionType,
                                                     llvm::Function::InternalLinkage,
                                                     functionName,
                                                     getModule());
        
        auto argList = prettyPrintFunction->arg_begin();
        llvm::Value *boolArg = argList++;
        
        llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", prettyPrintFunction);
        llvm::BasicBlock *oldInsertPoint = getIRBuilder()->GetInsertBlock();
        
        llvm::IRBuilder<> *builder = getIRBuilder();
        builder->SetInsertPoint(bodyBlock);
        
        llvm::BasicBlock *trueBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "true");
        llvm::BasicBlock *falseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "false");
        llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "after");
        
        builder->CreateCondBr(boolArg, trueBlock, falseBlock);
        prettyPrintFunction->getBasicBlockList().push_back(trueBlock);
        builder->SetInsertPoint(trueBlock);
        builder->CreateCall(Utilities::getPrintfFunction(getModule()), builder->CreateGlobalStringPtr("true"));
        builder->CreateBr(endBlock);
        
        prettyPrintFunction->getBasicBlockList().push_back(falseBlock);
        builder->SetInsertPoint(falseBlock);
        builder->CreateCall(Utilities::getPrintfFunction(getModule()), builder->CreateGlobalStringPtr("false"));
        builder->CreateBr(endBlock);
        
        prettyPrintFunction->getBasicBlockList().push_back(endBlock);
        builder->SetInsertPoint(endBlock);
        
        getIRBuilder()->CreateRetVoid();
        getIRBuilder()->SetInsertPoint(oldInsertPoint);
        
        return prettyPrintFunction;
    }
    
}; // namespace MAlice