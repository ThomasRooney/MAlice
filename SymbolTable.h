#ifndef _MALICESYMBOLTABLE
#define _MALICESYMBOLTABLE

#include <unordered_map>
#include <string>
#include "Entity.h"

namespace MAlice {

class SymbolTableNode
{
  private:
    std::unordered_map<std::string, Entity> m_scopeMap;
    SymbolTableNode *m_parentNode;
  public:
    SymbolTableNode(SymbolTableNode *parentNode);
    Entity* get(std::string);
    Entity* insert(std::string, Entity);
    unsigned int size();
    SymbolTableNode* createChildScope();
    SymbolTableNode* getChildScope();
    SymbolTableNode* getParentNode();
}; // class SymbolTableNode
    
}; // namespace MAlice

#endif