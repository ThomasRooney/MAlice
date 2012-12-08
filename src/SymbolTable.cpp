//
//  MAliceSymbolTable.cpp
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#include <iostream>
#include "SymbolTable.h"
#include <iterator>
#include <stdexcept>
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
            if (this->contains(identifier)) // Modified to stop run time stack corruption despite unoptimal
                entity = m_symbolMap.at(identifier);
            else
                return NULL;
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
    
    std::list<std::string> SymbolTable::getAllIdentifiers()
    {
        std::list<std::string> identifiers;
        
        for (auto pair = m_symbolMap.begin(); pair != m_symbolMap.end(); pair++) {
            identifiers.push_back((*pair).first);
        }
        
        return identifiers;
    }
}; // namespace MAlice
