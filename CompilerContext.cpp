
#include "CompilerContext.h"
#include "SymbolTable.h"

using namespace std;

namespace MAlice {
    
    void CompilerContext::addEntityInScope(std::string identifier, Entity entity)
    {
        if (m_symbolTables.empty())
            return;
        
        SymbolTable* & innermostSymbolTable = m_symbolTables.back();
        if (innermostSymbolTable->get(identifier))
            cerr << "Symbol conflict";
        
        innermostSymbolTable->insert(identifier, entity);
    }
    
    bool CompilerContext::isSymbolInScope(std::string identifier, Entity const **outEntity)
    {
        if (m_symbolTables.empty())
            return false;
        
        for (list<SymbolTable*>::reverse_iterator iterator = m_symbolTables.rbegin(); iterator != m_symbolTables.rend(); iterator++) {
            Entity const *entity = NULL;
            
            try {
                entity = (*iterator)->get(identifier);
            }
            catch(out_of_range e) {
                if (entity && outEntity) {
                    *outEntity = entity;
                    return true;
                }
            }
        }
        
        return false;
     }
    
    void CompilerContext::enterScope()
    {
        m_symbolTables.push_back(new SymbolTable());
    }
    
    void CompilerContext::exitScope()
    {
        // TODO: generate error if we pop too far.
        if (m_symbolTables.empty())
            return;
        
        SymbolTable* & lastSymbolTable = m_symbolTables.back();
        delete lastSymbolTable;
        
        m_symbolTables.pop_back();
    }
    
}; // namespace MAlice