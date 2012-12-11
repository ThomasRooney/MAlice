
#ifndef _MALICEDEBUG
#define _MALICEDEBUG

#include <iostream>

namespace MAlice {

    class SymbolTable;
    
    namespace Debug {

        static void printSymbolTableEntries(SymbolTable *symbolTable);
        
    }
    
}

#endif // #ifndef _MALICEDEBUG
