
#ifndef _MALICEERRORREPORTER
#define _MALICEERRORREPORTER

#include "MAliceParser.h"

#ifdef __cplusplus
#include <iostream>
#include <list>
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
        Warning,
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
        
        void printErrorHeader(Error *error);
        void printDecoratedLines(Error *error);
        void printDecoratedLine(Error *error, std::string line, unsigned int lineIndex);
        void decorateLineWithRanges(std::string *decorateLine, unsigned int lineIndex, std::list<Range*> ranges, char decorateCharacter);
        void getStartAndEndLinesForRanges(std::list<Range*> ranges, unsigned int startLine, unsigned int endLine, unsigned int *outStartLine, unsigned int *outEndLine);
        bool checkValidRanges(std::list<Range*> ranges);
    public:
        ErrorReporter();
        void reportError(Error *error);
        
        void setInput(std::string input);
        
        bool hasReportedErrors();
    }; // class ErrorReporter
    
}; // namespace MAlice
#endif

#endif /* #ifndef _MALICEERRORREPORTER */
