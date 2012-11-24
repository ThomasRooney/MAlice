
#ifndef _MALICECOMPILERCONTEXT
#define _MALICECOMPILERCONTEXT

#include <iostream>
#include <list>
#include <stdexcept>
#include <set>
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

namespace MAlice {

    class CompilerContext{
    private:
        std::list<SymbolTable*> m_symbolTables;
        std::set<std::string> m_keywords;
        ErrorReporter *m_errorReporter;
        MUTEX temporarySymbolTableLock;
        void configureKeywords();
        SymbolTable* t_symbolTable;
    public:
        CompilerContext();
        bool         lockTemporarySymbolTable();
        SymbolTable *getTemporarySymbolTable();
        bool         unlockTemporarySymbolTable();
        bool addEntityInScope(std::string identifier, Entity *entity);
        bool isSymbolInScope(std::string identifier, Entity **outEntity);
        bool isSymbolInCurrentScope(std::string identifier, Entity **outEntity);
        bool isKeyword(std::string string);
    
        void enterScope();
        void exitScope();
        
        ErrorReporter *getErrorReporter();
        void setErrorReporter(ErrorReporter *errorReporter);
    }; // class CompilerContext
    
}; // namespace MAlice

#endif /* #ifndef _MALICECOMPILERCONTEXT */
