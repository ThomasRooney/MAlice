
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
namespace MAlice {
    
    class ErrorReporter;
    
    ErrorReporter *getParserErrorReporter();
    void setParserErrorReporter(ErrorReporter *errorReporter);
    
    enum class ErrorType {
        Internal,
        Syntactic,
        Semantic,
        IO
    };
    
    class ErrorReporter {
    public:
        void reportError(ErrorType errorType, std::string errorMessage, bool isFatal);
        void reportError(unsigned int lineNumber, ErrorType errorType, std::string errorMessage, bool isFatal);
        void reportError(unsigned int lineNumber, unsigned int columnIndex, ErrorType errorType, std::string errorMessage, bool isFatal);
    }; // class ErrorReporter
    
}; // namespace MAlice
#endif

#endif /* #ifndef _MALICEERRORREPORTER */
