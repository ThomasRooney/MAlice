
#include "Range.h"

#include "ErrorPosition.h"

namespace MAlice {
    
    Range::Range(ErrorPosition startPosition, ErrorPosition endPosition) : m_startPosition(startPosition), m_endPosition(endPosition)
    {
    }
    
    ErrorPosition Range::getStartPosition()
    {
        return m_startPosition;
    }
    
    ErrorPosition Range::getEndPosition()
    {
        return m_endPosition;
    }
    
} // namespace MAlice