
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
    class CompilerContext;
    class Error;
    class ErrorPosition;
    
    ErrorReporter *getParserErrorReporter();
    void setParserErrorReporter(ErrorReporter *errorReporter);
    #pragma warning(disable:4482)
    enum ErrorType {
        Internal,
        Lexical,
        Syntactic,
        Semantic,
        IO
    };
    
    class ErrorReporter {
    private:
        std::string m_input;
        bool m_hasReportedErrors;
        
        std::string getLineOfInput(unsigned int lineNumber);
        
        void printLineWithArrow(ErrorPosition *errorPosition);
        void printLineWithUnderline(std::string line, Range *range);
        void printErrorHeader(Error *error);
    public:
        ErrorReporter();
        void reportError(Error *error);
        
        void setInput(std::string input);
        
        bool hasReportedErrors();
    }; // class ErrorReporter
    
}; // namespace MAlice
#endif

#endif /* #ifndef _MALICEERRORREPORTER */
