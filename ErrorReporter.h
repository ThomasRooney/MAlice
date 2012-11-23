
#ifndef _MALICEERRORREPORTER
#define _MALICEERRORREPORTER

#include <iostream>

namespace MAlice {
    
    typedef enum {
        ErrorTypeSyntactic,
        ErrorTypeSemantic
    } ErrorType;
    
    class ErrorReporter {
    private:
        std::string m_input;
    public:
        ErrorReporter(std::string input);
        
        void reportError(ErrorType errorType, std::string errorMessage, bool isFatal);
        void reportError(unsigned int lineNumber, unsigned int columnIndex, ErrorType errorType, std::string errorMessage, bool isFatal);
        
    }; // class ErrorReporter
    
}; // namespace MAlice

#endif /* #ifndef _MALICEERRORREPORTER */
