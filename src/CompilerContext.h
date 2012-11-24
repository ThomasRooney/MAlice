
#ifndef _MALICECOMPILERCONTEXT
#define _MALICECOMPILERCONTEXT

#include <iostream>
#include <list>
#include <stdexcept>

#include "SymbolTable.h"

namespace MAlice {

    class CompilerContext{
    private:
        std::list<SymbolTable*> m_symbolTables;
        
    public:
        CompilerContext();
        
        void addEntityInScope(std::string identifier, Entity entity);
        bool isSymbolInScope(std::string identifier, Entity const **outEntity);
    
        void enterScope();
        void exitScope();
    }; // class CompilerContext
    
}; // namespace MAlice

#endif /* #ifndef _MALICECOMPILERCONTEXT */
