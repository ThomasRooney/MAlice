
#ifndef _MALICERANGE
#define _MALICERANGE

#include <iostream>
#include "ErrorPosition.h"

namespace MAlice {
    
    class Range {
    private:
        ErrorPosition m_startPosition;
        ErrorPosition m_endPosition;
        
    public:
        Range(ErrorPosition startPosition, ErrorPosition endPosition);
        
        ErrorPosition getStartPosition();
        ErrorPosition getEndPosition();
    };
    
} // namespace MAlice

#endif /* #ifndef _MALICERANGE */
