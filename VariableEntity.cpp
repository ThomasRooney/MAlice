//
//  VariableEntity.cpp
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#include "VariableEntity.h"

namespace MAlice {

VariableEntity::VariableEntity(std::string identifier, unsigned short type) : Entity(identifier)
{
    m_type = type;
}

};