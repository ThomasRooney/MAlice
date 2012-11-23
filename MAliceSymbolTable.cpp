//
//  MAliceSymbolTable.cpp
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#include "MAliceSymbolTable.h"

MAliceSymbolTableNode::MAliceSymbolTableNode(MAliceSymbolTableNode *parentNode)
{
    m_parentNode = parentNode;
}
