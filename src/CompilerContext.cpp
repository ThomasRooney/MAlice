
#include <unordered_map>

#include "CompilerContext.h"

#include "ErrorFactory.h"
#include "SymbolTable.h"
#include "FunctionProcedureEntity.h"
#include "llvm/LLVMContext.h"


using namespace std;

namespace MAlice {
    
    CompilerContext::CompilerContext(std::string input)
    {
        m_input = input;
        withinExpressionTree = false;
        // Set the ANTLR structure defaults
        m_lexer = NULL;
        m_parser = NULL;
        m_inputStream = NULL;
        m_tokenStream = NULL;
        
        m_symbolTables.push_back(new SymbolTable());
        t_symbolTable = new SymbolTable();
        m_errorReporter = NULL;
        configureKeywords();
        #ifdef _WIN32
           temporarySymbolTableLock = CreateMutex(0, FALSE, 0);
        #else
           pthread_mutex_init (&temporarySymbolTableLock, NULL);;
        #endif
        
        m_irBuilder = new llvm::IRBuilder<>(llvm::getGlobalContext());
    }
    
    CompilerContext::~CompilerContext()
    {
        for (list<SymbolTable*>::iterator it = m_symbolTables.begin(); it != m_symbolTables.end(); ++it)
        {
            SymbolTable *table = *it;

            delete table, table = NULL;
        }
        m_symbolTables.clear();
        // (Dangling pointers to symbol table pointers.. clear them.)
        m_functionProcedureScope.clear();
        
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

    void CompilerContext::enterScope()
    {
        m_symbolTables.push_back(new SymbolTable());
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
        if (m_functionProcedureScope.empty())
            return NULL;
        
        return m_functionProcedureScope.back();
    }
    
    void CompilerContext::pushFunctionProcedureEntity(FunctionProcedureEntity *entity)
    {
        m_functionProcedureScope.push_back(entity);
    }
    
    void CompilerContext::popFunctionProcedureEntity()
    {
        if (m_functionProcedureScope.empty())
            return;
        m_functionProcedureScope.pop_back();
        
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

}; // namespace MAlice