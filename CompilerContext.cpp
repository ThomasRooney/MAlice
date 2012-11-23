
#include "CompilerContext.h"

using namespace std;

namespace MAlice {
    
    void CompilerContext::addEntityInScope(std::string identifier, Entity entity)
    {
        if (m_symbolTables.empty())
            return;
        
        SymbolTable& innermostSymbolTable = m_symbolTables.back();
        if (innermostSymbolTable.get(identifier))
            cerr << "Symbol conflict";
        
        innermostSymbolTable.insert(identifier, entity);
    }
    
}; // namespace MAlice