//
//  MAliceSymbolTable.cpp
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#include "SymbolTable.h"
#include <iterator>
#include <vector>
#include <unordered_map>

using namespace std;

namespace MAlice {
    SymbolTable::~SymbolTable()
    {
        vector<string> keys;
        
        for (unordered_map<string, Entity*>::iterator it = m_symbolMap.begin(); it != m_symbolMap.end(); ++it) {
            keys.push_back(it->first);
        }
        
        for (vector<string>::iterator it = keys.begin(); it != keys.end(); ++it) {
            Entity *entity = m_symbolMap[*it];
            
            if (entity != NULL) {
                delete entity;
                entity = NULL;
            }
        }
        
        m_symbolMap.clear();
    }
    // Throws out of range exception to calling function
    Entity *SymbolTable::get(string identifier)
    {
        Entity *entity = NULL;
        
        try {
            entity = m_symbolMap.at(identifier);
        }
        catch(out_of_range e) {
            return NULL;
        }
        
        return entity;
    }

    bool SymbolTable::contains(string identifier)
    {
      return (m_symbolMap.find(identifier) != m_symbolMap.end());
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
