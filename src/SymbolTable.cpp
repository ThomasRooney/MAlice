//
//  MAliceSymbolTable.cpp
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#include "SymbolTable.h"
#include <iterator>

using namespace std;

namespace MAlice {
    SymbolTable::~SymbolTable()
    {
        for(auto element = (m_symbolMap.end()); element != m_symbolMap.begin(); ) {
            element--;
            if ((*element).second != NULL)
               delete (*element).second;
            (*element).second = NULL;
        }
        m_symbolMap.clear();
        
    }
    
    Entity *SymbolTable::get(string identifier)
    {
        return m_symbolMap[identifier];
    }
    
    void SymbolTable::insert(string identifier, Entity *entity)
    {
        m_symbolMap[identifier] = entity;
    }
    
    void SymbolTable::clear(){
      m_symbolMap.clear();
    }
    
    unsigned int SymbolTable::numberOfSymbols()
    {
        return (unsigned int)m_symbolMap.size();
    }
}; // namespace MAlice
