
#include "ErrorReporter.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <sstream>
#include "limits.h"
#include "MAliceParser.h"

#define LINE_NUMBER_NA      UINT_MAX
#define COL_INDEX_NA        UINT_MAX

using namespace std;

static MAlice::ErrorReporter *parserErrorReporter = NULL;

void handleParserError(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer, pANTLR3_UINT8 * tokenNames)
{
    ANTLR3_EXCEPTION_struct *exception = recognizer->state->exception;
    
    string errorMessage;
    MAlice::ErrorType errorType;
    bool isErrorFatal = false;
    
    unsigned int lineNumber = UINT_MAX;
    unsigned int characterIndex = UINT_MAX;
    pANTLR3_COMMON_TOKEN token = (pANTLR3_COMMON_TOKEN)exception->token;
    
    switch (exception->type)
    {
        case ANTLR3_RECOGNITION_EXCEPTION:
        {
            string identifier = (char*)token->getText(token)->chars;
            
            errorMessage = "Unrecognised token '" + identifier + "'.";
            errorType = MAlice::ErrorType::Syntactic;
            lineNumber = token->line;
            characterIndex = token->charPosition;
        }
            break;
        case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
            break;
        case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
        {
            errorType = MAlice::ErrorType::Syntactic;
            errorMessage = "Unrecognised input.";
            lineNumber = token->line;
        }
            break;
        case ANTLR3_MISMATCHED_SET_EXCEPTION:
            break;
        case ANTLR3_EARLY_EXIT_EXCEPTION:
            break;
        case ANTLR3_FAILED_PREDICATE_EXCEPTION:
            errorType = MAlice::ErrorType::Internal;
            isErrorFatal = true;
            break;
        default:
            break;
    }
    
    if (lineNumber != UINT_MAX && characterIndex != UINT_MAX) {
        parserErrorReporter->reportError(lineNumber,
                                         characterIndex,
                                         errorType,
                                         errorMessage,
                                         isErrorFatal);
    } else if (lineNumber != UINT_MAX) {
        parserErrorReporter->reportError(lineNumber,
                                         errorType,
                                         errorMessage,
                                         isErrorFatal);
    } else {
        parserErrorReporter->reportError(lineNumber,
                                         characterIndex,
                                         errorType,
                                         errorMessage,
                                         isErrorFatal);
    }
}

void handleLexerError(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer, pANTLR3_UINT8 * tokenNames)
{
}

namespace MAlice {
    
    ErrorReporter *getParserErrorReporter()
    {
        return parserErrorReporter;
    }
    
    void setParserErrorReporter(ErrorReporter *errorReporter)
    {
        parserErrorReporter = errorReporter;
    }
    
    void ErrorReporter::reportError(ErrorType errorType, string errorMessage, bool isFatal)
    {
        reportError(LINE_NUMBER_NA, COL_INDEX_NA, errorType, errorMessage, isFatal);
    }
    
    void ErrorReporter::reportError(unsigned int lineNumber, ErrorType errorType, string errorMessage, bool isFatal)
    {
        reportError(lineNumber, COL_INDEX_NA, errorType, errorMessage, isFatal);
    }
    
    void ErrorReporter::reportError(unsigned int lineNumber, unsigned int columnIndex, ErrorType errorType, string errorMessage, bool isFatal)
    {
        switch(errorType)
        {
            case ErrorType::Internal:
                cerr << "Internal error: ";
                break;
            case ErrorType::Syntactic:
                cerr << "Syntactic error: ";
                break;
            case ErrorType::Semantic:
                cerr << "Semantic error: ";
                break;
            case ErrorType::IO:
                cerr << "IO error: ";
                break;
        }
        
        if (lineNumber != LINE_NUMBER_NA || columnIndex != COL_INDEX_NA) {
            cerr << "(";
            
            if (lineNumber != LINE_NUMBER_NA)
                cerr << "Line " << lineNumber;
            
            if (lineNumber != COL_INDEX_NA)
                cerr << ", Col " << columnIndex;
            
            cerr << ") ";
        }
        
        cerr << "\n";
        string messageString = errorMessage.c_str();
        
        std::stringstream stringStream(messageString);
        std::string line;
        while(std::getline(stringStream, line, '\n')) {
            cerr << "  " << line << endl;
        }
        
        if (isFatal)
            exit(EXIT_FAILURE);
    }
    
}; // namespace ErrorReporter