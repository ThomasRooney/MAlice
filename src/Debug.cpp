
#include <vector>

#include "Debug.h"

#include "SymbolTable.h"

namespace MAlice {
    
    void Debug::printSymbolTableEntries(SymbolTable *symbolTable)
    {
        std::vector<std::string> identifiers = symbolTable->getAllIdentifiers();
        
        for (std::vector<std::string>::iterator it = identifiers.begin(); it != identifiers.end(); ++it) {
            std::cout << *it << ": " << symbolTable->get(*it) << "\n";
        }
    }
    
    
} // namespace MAlice