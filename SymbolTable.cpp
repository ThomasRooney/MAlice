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
    const Entity *SymbolTable::get(string identifier)
    {
        unordered_map<string, Entity>::const_iterator iterator = m_symbolMap.find(identifier);
        if (iterator == m_symbolMap.end())
            return NULL;
        
        return &iterator->second;
    }
    
    void SymbolTable::insert(string identifier, Entity entity)
    {
        m_symbolMap.insert({identifier, entity});
    }
    
    unsigned int SymbolTable::numberOfSymbols()
    {
        return (unsigned int)m_symbolMap.size();
    }
}; // namespace MAlice
