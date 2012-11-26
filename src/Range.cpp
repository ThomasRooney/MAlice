//
//  Range.cpp
//  MAlice
//
//  Created by Alex Rozanski on 26/11/2012.
//
//

#include "Range.h"

namespace MAlice {
    
    Range::Range()
    {
        m_location = 0;
        m_length = 0;
    }
    
    Range::Range(unsigned int location, unsigned int length)
    {
        m_location = location;
        m_length = length;
    }
    
    unsigned int Range::getLocation()
    {
        return m_location;
    }
    
    unsigned int Range::getLength()
    {
        return m_length;
    }
    
    void Range::setLocation(unsigned int location)
    {
        m_location = location;
    }
    
    void Range::setLength(unsigned int length)
    {
        m_length = length;
    }
    
} // namespace MAlice