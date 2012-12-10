#ifndef _MALICESYMBOLTABLE
#define _MALICESYMBOLTABLE

#include <unordered_map>
#include <string>
#include <vector>
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
    void clear();
    bool contains(std::string);
    unsigned int numberOfSymbols();
    
    std::vector<std::string> getAllIdentifiers();
}; // class SymbolTableNode
    
}; // namespace MAlice

#endif