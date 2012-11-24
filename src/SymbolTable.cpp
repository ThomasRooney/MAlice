//
//  MAliceSymbolTable.cpp
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#include "SymbolTable.h"

using namespace std;

namespace MAlice {
    SymbolTable::~SymbolTable()
    {
        for(auto& element = m_symbolMap.rbegin(); element != m_symbolMap.rend(); ) {
            if ((*element).second != NULL)
               delete (*element).second;
            (*element).second = NULL;
            m_symbolMap.erase((*element).first);
        }
    }
    
    Entity *SymbolTable::get(string identifier)
    {
        return m_symbolMap[identifier];
    }
    
    void SymbolTable::insert(string identifier, Entity *entity)
    {
        m_symbolMap[identifier] = entity;
    }
    
    unsigned int SymbolTable::numberOfSymbols()
    {
        return (unsigned int)m_symbolMap.size();
    }
}; // namespace MAlice
