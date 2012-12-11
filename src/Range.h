
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
        
        static Range *create(unsigned int startLine, unsigned int startColumn);
        static Range *create(unsigned int startLine, unsigned int startColumn, unsigned int endColumn);
        static Range *create(unsigned int startLine, unsigned int startColumn, unsigned int endLine, unsigned int endColumn);
        
        ErrorPosition getStartPosition();
        ErrorPosition getEndPosition();
    };
    
} // namespace MAlice

#endif /* #ifndef _MALICERANGE */
