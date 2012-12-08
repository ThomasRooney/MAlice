
#ifndef _MALICECOMPILERCONTEXT
#define _MALICECOMPILERCONTEXT
#include "llvmHeader.h"
#include <iostream>
#include <list>
#include <string>
#include <stdexcept>
#include <set>
#include <stack>
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

#include "llvm/Support/IRBuilder.h"

namespace MAlice {

    class FunctionProcedureEntity;
    
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
        
        std::list<FunctionProcedureEntity*> m_functionProcedureScope;
        std::stack<llvm::IRBuilderBase::InsertPoint*> m_insertionPoints;
        
        std::string getLineOfInput(unsigned int lineNumber);
        
        // For code generation
        llvm::IRBuilder<> *m_irBuilder;
        llvm::Module *m_module;
        
    public:
        CompilerContext(std::string input);
        ~CompilerContext();
        bool         lockTemporarySymbolTable();
        SymbolTable *getTemporarySymbolTable();
        bool         unlockTemporarySymbolTable();
        bool addEntityInScope(std::string identifier, Entity *entity);
        bool isSymbolInScope(std::string identifier, Entity **outEntity);
        bool isSymbolInCurrentScope(std::string identifier, Entity **outEntity);
        bool isKeyword(std::string string);
    
        void enterScope();
        void exitScope();

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
        
        llvm::IRBuilder<> *getIRBuilder();
        llvm::Module *getModule();
        
        void pushCurrentInsertionPoint();
        llvm::IRBuilderBase::InsertPoint *popInsertionPoint();
    
    }; // class CompilerContext
    
}; // namespace MAlice

#endif /* #ifndef _MALICECOMPILERCONTEXT */
