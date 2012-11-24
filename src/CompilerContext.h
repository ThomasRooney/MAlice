
#ifndef _MALICECOMPILERCONTEXT
#define _MALICECOMPILERCONTEXT

#include <iostream>
#include <list>
#include <stdexcept>
#include <set>

#include "ErrorReporter.h"
#include "SymbolTable.h"

namespace MAlice {

    class CompilerContext{
    private:
        std::list<SymbolTable*> m_symbolTables;
        std::set<std::string> m_keywords;
        ErrorReporter *m_errorReporter;
        
        void configureKeywords();
        
    public:
        CompilerContext();
        
        void addEntityInScope(std::string identifier, Entity *entity);
        bool isSymbolInScope(std::string identifier, Entity **outEntity);
        bool isKeyword(std::string string);
    
        void enterScope();
        void exitScope();
        
        ErrorReporter *getErrorReporter();
        void setErrorReporter(ErrorReporter *errorReporter);
    }; // class CompilerContext
    
}; // namespace MAlice

#endif /* #ifndef _MALICECOMPILERCONTEXT */
