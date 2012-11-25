
#include "ErrorReporter.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include "limits.h"
#include "MAliceParser.h"

#define LINE_NUMBER_NA      UINT_MAX
#define COL_INDEX_NA        UINT_MAX

using namespace std;

static MAlice::ErrorReporter *parserErrorReporter = NULL;

void handleParserError(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer, pANTLR3_UINT8 * tokenNames)
{
    if (!parserErrorReporter)
        return;
    
    ANTLR3_EXCEPTION_struct *exception = recognizer->state->exception;
    pANTLR3_COMMON_TOKEN token = (pANTLR3_COMMON_TOKEN)exception->token;
    
    switch (exception->type)
    {
        case ANTLR3_RECOGNITION_EXCEPTION:
        {
            string identifier = (char*)token->getText(token)->chars;
            string errorMessage = "Unrecognised token '" + identifier + "'.";
            
            parserErrorReporter->reportError(token->line, token->charPosition, MAlice::ErrorType::Syntactic, errorMessage, true);
        }
            break;
        case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
            break;
        case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
        {
            string tokenText = (char*)token->toString(token);
            string errorMessage = "Unrecognised input.";
            
            parserErrorReporter->reportError(token->line, MAlice::ErrorType::Syntactic, errorMessage, true);
        }
            break;
        case ANTLR3_MISMATCHED_SET_EXCEPTION:
            break;
        case ANTLR3_EARLY_EXIT_EXCEPTION:
            break;
        case ANTLR3_FAILED_PREDICATE_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Internal, "", true);
        }
            break;
        default:
            break;
    }
}

void handleLexerError(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer, pANTLR3_UINT8 * tokenNames)
{
    if (!parserErrorReporter)
        return;
    
    ANTLR3_EXCEPTION_struct *exception = recognizer->state->exception;
    pANTLR3_COMMON_TOKEN token = (pANTLR3_COMMON_TOKEN)exception->token;
    
    switch (exception->type)
    {
        case ANTLR3_RECOGNITION_EXCEPTION:
        {
            string identifier = (char*)token->getText(token)->chars;
            string errorMessage = "Unrecognised token '" + identifier + "'.";
            
            parserErrorReporter->reportError(token->line, token->charPosition, MAlice::ErrorType::Syntactic, errorMessage, true);
        }
            break;
        case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
            break;
        case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
        {
            string errorMessage = "Unrecognised input.";
            
            parserErrorReporter->reportError(exception->line, exception->charPositionInLine, MAlice::ErrorType::Syntactic, errorMessage, true);
        }
            break;
        case ANTLR3_MISMATCHED_SET_EXCEPTION:
            break;
        case ANTLR3_EARLY_EXIT_EXCEPTION:
            break;
        case ANTLR3_FAILED_PREDICATE_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Internal, "", true);
        }
            break;
        default:
            break;
    }
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
                cerr << "Internal error";
                break;
            case ErrorType::Syntactic:
                cerr << "Syntactic error";
                break;
            case ErrorType::Semantic:
                cerr << "Semantic error";
                break;
            case ErrorType::IO:
                cerr << "IO error";
                break;
        }
        
        if (lineNumber != LINE_NUMBER_NA || columnIndex != COL_INDEX_NA) {
            cerr << " (";
            
            if (lineNumber != LINE_NUMBER_NA)
                cerr << "Line " << lineNumber;
            
            cerr << ")";
        }
        
        cerr << ": ";
        
        string messageString = errorMessage.c_str();
        cerr << messageString;
        
        if (columnIndex != COL_INDEX_NA) {
            std::string line = getLineOfInput(lineNumber - 1);
            
            cerr << "\n  " << line << "\n  ";
            cerr << setw(columnIndex+1) << "^";
        } else if (lineNumber != LINE_NUMBER_NA) {
            std::string line = getLineOfInput(lineNumber - 1);
            cerr << "\n  " << line << "\n";
        }
        
        cerr << endl;
        
        if (isFatal)
            exit(EXIT_FAILURE);
    }
    
    void ErrorReporter::setInput(std::string input)
    {
        m_input = input;
    }
    
    std::string ErrorReporter::getLineOfInput(unsigned int lineNumber)
    {
        if (m_input.empty())
            return "";
        
        std::stringstream stringStream(m_input);
        std::string line;
        
        unsigned int i = 0;
        while(std::getline(stringStream, line, '\n')) {
            if (i == lineNumber)
                return line;
            
            ++i;
        }
        
        return "";
    }
    
}; // namespace ErrorReporter