
#include "ErrorPosition.h"

#include "limits.h"

namespace MAlice {
    
    ErrorPosition::ErrorPosition(unsigned int lineNumber)
    {
        m_lineNumber = lineNumber;
        m_columnIndex = UINT_MAX;
    }
    
    ErrorPosition::ErrorPosition(unsigned int lineNumber, unsigned int columnIndex)
    {
        m_lineNumber = lineNumber;
        m_columnIndex = columnIndex;
    }
    
    bool ErrorPosition::hasColumnIndex()
    {
        return m_columnIndex != UINT_MAX;
    }
    
    unsigned int ErrorPosition::getLineNumber()
    {
        return m_lineNumber;
    }
    
    unsigned int ErrorPosition::getColumnIndex()
    {
        return m_columnIndex;
    }
    
} // namespace MAlice