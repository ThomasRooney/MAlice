
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
    
    Range *Range::create(unsigned int startLine, unsigned int startColumn)
    {
        return create(startLine, startColumn, startLine, startColumn);
    }
    
    Range *Range::create(unsigned int startLine, unsigned int startColumn, unsigned int endColumn)
    {
        return create(startLine, startColumn, startLine, endColumn);
    }
    
    Range *Range::create(unsigned int startLine, unsigned int startColumn, unsigned int endLine, unsigned int endColumn)
    {
        return new Range(ErrorPosition(startLine, startColumn), ErrorPosition(endLine, endColumn));
    }
    
} // namespace MAlice