
#ifndef _MALICEERRORPOSITION
#define _MALICEERRORPOSITION

#include <iostream>

namespace MAlice {
    
    class ErrorPosition {
    private:
        unsigned int m_lineNumber;
        unsigned int m_columnIndex;
    public:
        ErrorPosition(unsigned int lineNumber);
        ErrorPosition(unsigned int lineNumber, unsigned int columnIndex);
        
        bool hasColumnIndex();
        
        unsigned int getLineNumber();
        unsigned int getColumnIndex();
    };
    
} // namespace MAlice

#endif /* #ifndef _MALICEERRORPOSITION */
