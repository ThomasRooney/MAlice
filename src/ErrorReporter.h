
#ifndef _MALICEERRORREPORTER
#define _MALICEERRORREPORTER

#include "MAliceParser.h"

#ifdef __cplusplus
#include <iostream>
#endif


#ifdef __cplusplus
extern "C" {
#endif
    extern void handleParserError(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer, pANTLR3_UINT8 * tokenNames);
    extern void handleLexerError(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer, pANTLR3_UINT8 * tokenNames);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "Range.h"

namespace MAlice {
    
    class ErrorReporter;
    
    ErrorReporter *getParserErrorReporter();
    void setParserErrorReporter(ErrorReporter *errorReporter);
    #pragma warning(disable:4482)
    enum ErrorType {
        Internal,
        Syntactic,
        Semantic,
        IO
    };
    
    class ErrorReporter {
    private:
        std::string m_input;
        bool m_hasReportedErrors;
        
        std::string getLineOfInput(unsigned int lineNumber);
        
        void printLineWithArrow(std::string line, unsigned int arrowPosition);
        void printLineWithUnderline(std::string line, Range range);
        void printErrorHeader(MAlice::ErrorType errorType, unsigned int lineNumber, unsigned int columnIndex, std::string errorMessage);
    public:
        ErrorReporter();
        void reportError(ErrorType errorType, std::string errorMessage, bool isFatal);
        void reportError(unsigned int lineNumber, ErrorType errorType, std::string errorMessage, bool isFatal);
        void reportError(unsigned int lineNumber, unsigned int columnIndex, ErrorType errorType, std::string errorMessage, bool isFatal);
        void reportError(unsigned int lineNumber, unsigned int columnIndex, ErrorType errorType, std::string errorMessage, std::string additionalInformation, bool isFatal);
        void reportError(unsigned int lineNumber, Range range, ErrorType errorType, std::string errorMessage, std::string additionalInformation, bool isFatal);
        void setInput(std::string input);
        
        bool hasReportedErrors();
    }; // class ErrorReporter
    
}; // namespace MAlice
#endif

#endif /* #ifndef _MALICEERRORREPORTER */
