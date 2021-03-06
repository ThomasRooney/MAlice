
#ifndef _MALICECOMPILERCONTEXT
#define _MALICECOMPILERCONTEXT
#include "LLVMHeader.h"
#include <iostream>
#include <list>
#include <string>
#include <stdexcept>
#include <set>
#include <stack>
#include <unordered_map>
#ifdef _WIN32
#include <windows.h>
#include <process.h>			//Data types
#define MUTEX HANDLE
#else
#include <pthread.h>
#define MUTEX pthread_mutex_t
#endif

#include "ErrorReporter.h"
#include "SymbolTable.h"

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include "Types.h"

namespace MAlice {

    class FunctionProcedureEntity;
    class IdentifierDispenser;
    class Type;
    class VariableEntity;
    class StringTable;
    
    class CompilerContext{
    private:
        std::string m_input;
        bool withinExpressionTree;
        pMAliceLexer m_lexer;
        pMAliceParser m_parser;
        pANTLR3_INPUT_STREAM m_inputStream;
        pANTLR3_COMMON_TOKEN_STREAM m_tokenStream;
        
        std::list<SymbolTable*> m_symbolTables;
        std::set<std::string> m_keywords;
        ErrorReporter *m_errorReporter;
        MUTEX temporarySymbolTableLock;
        void configureKeywords();
        SymbolTable* t_symbolTable;
    
        std::stack<FunctionProcedureEntity*> m_functionProcedureScopeStack;
        std::stack<llvm::BasicBlock*> m_insertPointStack;
        
        std::string getLineOfInput(unsigned int lineNumber);
        
        // For code generation
        llvm::IRBuilder<> *m_irBuilder;
        llvm::Module *m_module;
        // For debugging information
        bool m_optimisationsOn;
        llvm::DIBuilder* m_DebugBuilder;
        llvm::MDNode *m_dbGlobalScope;
        llvm::DIFile m_dbfile;
        std::vector<llvm::MDNode*> m_dbgScope;
        
        
        IdentifierDispenser *m_identifierDispenser;
        std::unordered_map<unsigned int, llvm::Value*> m_ioFormatStringMap;
        
        StringTable *m_stringTable;
        
        void initialiseCompilerContext(std::string srcInput);
        
    public:
        CompilerContext(std::string input);
        CompilerContext(std::string input, std::string sourceFile, std::string dirFile); // With debug information (extension)
        ~CompilerContext();
        bool         lockTemporarySymbolTable();
        void         clearSemanticInformation();
        SymbolTable *getTemporarySymbolTable();
        bool         unlockTemporarySymbolTable();
        bool addEntityInScope(std::string identifier, Entity *entity);
        bool isSymbolInScope(std::string identifier, Entity **outEntity);
        bool isSymbolInCurrentScope(std::string identifier, Entity **outEntity);
        bool isKeyword(std::string string);
        std::vector<VariableEntity*> variableEntitiesInCurrentScope();

        void enterDebugScope(ASTNode);
        void enterDebugScope(llvm::MDNode* node);
        void exitDebugScope();    
        llvm::MDNode* getCurrentDBScope();
        void enterScope();
        void exitScope();
        void enterFunctionProcedureScope(FunctionProcedureEntity *funcProcEntity);
        void exitFunctionProcedureScope();

        void beginExpression();
        bool withinExpression();
        void endExpression();

        ErrorReporter *getErrorReporter();
        void setErrorReporter(ErrorReporter *errorReporter);
        
        pMAliceLexer getLexer();
        void setLexer(pMAliceLexer lexer);
        pMAliceParser getParser();
        void setParser(pMAliceParser parser);
        pANTLR3_INPUT_STREAM getInputStream();
        void setInputStream(pANTLR3_INPUT_STREAM inputStream);
        pANTLR3_COMMON_TOKEN_STREAM getTokenStream();
        void setTokenStream(pANTLR3_COMMON_TOKEN_STREAM tokenStream);
        
        FunctionProcedureEntity *getCurrentFunctionProcedureEntity();
        void pushFunctionProcedureEntity(FunctionProcedureEntity *entity);
        void popFunctionProcedureEntity();
        
        void saveInsertPoint();
        void restoreInsertPoint();
        
        llvm::IRBuilder<> *getIRBuilder();
        llvm::Module *getModule();

        llvm::DIBuilder *getDGBuilder();
        void setCurrentDBScope(llvm::MDNode *s);
        llvm::DIFile *getDIFile();
        
        IdentifierDispenser *getIdentifierDispenser();
        void setIdentifierDispenser(IdentifierDispenser *dispenser);
        
        llvm::Value *ioFormatStringForExpressionType(Type type);
        llvm::Function *prettyPrintBoolFunction();
        
        StringTable *getStringTable();
        
        bool getOptimisationsOn();
        void setOptimisationsOn(bool optimisationsOn);
    
    }; // class CompilerContext
    
}; // namespace MAlice

#endif /* #ifndef _MALICECOMPILERCONTEXT */

