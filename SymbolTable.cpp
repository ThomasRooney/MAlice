//
//  MAliceSymbolTable.cpp
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#include "SymbolTable.h"

namespace MAlice {

SymbolTableNode :: SymbolTableNode(SymbolTableNode *parentNode)
{
    m_parentNode = parentNode;
}
    
}; // namespace MAlice
