//
//  Range.h
//  MAlice
//
//  Created by Alex Rozanski on 26/11/2012.
//
//

#ifndef _MALICERANGE
#define _MALICERANGE

#include <iostream>

namespace MAlice {
    
    class Range {
    private:
        unsigned int m_location;
        unsigned int m_length;
        
    public:
        Range();
        Range(unsigned int location, unsigned int length);
        
        unsigned int getLocation();
        void setLocation(unsigned int location);
        unsigned int getLength();
        void setLength(unsigned int location);
    };
    
} // namespace MAlice

#endif /* #ifndef _MALICERANGE */
