
#ifndef _MALICECOMPILERCONTEXT
#define _MALICECOMPILERCONTEXT

#include <iostream>
#include <list>

#include "SymbolTable.h"

namespace MAlice {

    class CompilerContext{
    private:
        std::list<SymbolTable> m_symbolTables;
    }; // class CompilerContext
    
}; // namespace MAlice

#endif /* #ifndef _MALICECOMPILERCONTEXT */
