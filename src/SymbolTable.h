#ifndef _MALICESYMBOLTABLE
#define _MALICESYMBOLTABLE

#include <unordered_map>
#include <string>
#include "Entity.h"

namespace MAlice {

class SymbolTable
{
  private:
    std::unordered_map<std::string, Entity*> m_symbolMap;
    
  public:
    Entity *get(std::string);
    ~SymbolTable();
    void insert(std::string, Entity*);
    unsigned int numberOfSymbols();
}; // class SymbolTableNode
    
}; // namespace MAlice

#endif