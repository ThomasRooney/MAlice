
#include "ErrorReporter.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include "limits.h"
#include "Utilities.h"
#include "MAliceParser.h"
#include "Types.h"

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
            string errorMessage = "Unexpected token '" + identifier + "'.";
            
            parserErrorReporter->reportError(token->line, token->charPosition, MAlice::ErrorType::Syntactic, errorMessage, false);
        }
            break;
        case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Syntactic, "Mismatched token exception", false);
        }
            break;
        case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
        {
            string tokenText = (char*)token->toString(token);
            string errorMessage = "Unrecognised or missing token.";
            
            parserErrorReporter->reportError(token->line, MAlice::ErrorType::Syntactic, errorMessage, false);
        }
            break;
        case ANTLR3_MISMATCHED_SET_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Syntactic, "Mismatched set exception", false);
        }
            break;
        case ANTLR3_EARLY_EXIT_EXCEPTION:
        {
            string errorMessage;
            
            if (token && token->type == EOF) {
                errorMessage = "Unexpected end of input.";
                
                parserErrorReporter->reportError(MAlice::ErrorType::Syntactic, errorMessage, true);
            } else {
                string identifier = (char*)token->getText(token)->chars;
                errorMessage = "Unexpected token '" + identifier + "'.";
                
                parserErrorReporter->reportError(token->line, token->charPosition, MAlice::ErrorType::Syntactic, errorMessage, true);
            }
        }
            break;
        case ANTLR3_FAILED_PREDICATE_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Internal, "", true);
        }
            break;
        case ANTLR3_MISSING_TOKEN_EXCEPTION:
        {            
            ANTLR3_UINT32 expectedToken = exception->expecting;
            
            std::string expectedTokenText = MAlice::Utilities::getTokenTextFromTokenIdentifier(expectedToken);
            std::string errorMessage;
            
            if (!expectedTokenText.empty())
                errorMessage = "Expected token '" + expectedTokenText + "'.";
            else
                errorMessage = "Expected token missing from input.";
            
            if (token) {
                parserErrorReporter->reportError(token->line, token->charPosition, MAlice::ErrorType::Syntactic, errorMessage, true);
            }
            else {
                parserErrorReporter->reportError(MAlice::ErrorType::Syntactic, errorMessage, true);
            }
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
            if (token != NULL) {
                string identifier = (char*)token->getText(token)->chars;
                string errorMessage = "Unexpected token '" + identifier + "'.";
                
                parserErrorReporter->reportError(token->line, token->charPosition, MAlice::ErrorType::Lexical, errorMessage, true);
            } else {
                parserErrorReporter->reportError(MAlice::ErrorType::Lexical, "Unrecognised token", true);
            }
        }
            break;
        case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Syntactic, "Mismatched token exception", true);
        }
            break;
        case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
        {
            string errorMessage = "Unrecognised or missing token.";
            
            parserErrorReporter->reportError(exception->line, exception->charPositionInLine, MAlice::ErrorType::Syntactic, errorMessage, true);
        }
            break;
        case ANTLR3_MISMATCHED_SET_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Lexical, "Mismatched set exception", true);
        }
            break;
        case ANTLR3_EARLY_EXIT_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Lexical, "Early exit exception", true);
        }
            break;
        case ANTLR3_FAILED_PREDICATE_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorType::Lexical, "", true);
        }
            break;
        default:
            break;
    }
}

namespace MAlice {
    
    ErrorReporter::ErrorReporter()
    {
        m_hasReportedErrors = false;
    }

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
        reportError(LINE_NUMBER_NA, COL_INDEX_NA, errorType, errorMessage, "", isFatal);
    }
    
    void ErrorReporter::reportError(unsigned int lineNumber, ErrorType errorType, string errorMessage, bool isFatal)
    {
        reportError(lineNumber, COL_INDEX_NA, errorType, errorMessage, "", isFatal);
    }
    
    void ErrorReporter::reportError(unsigned int lineNumber, unsigned int columnIndex, ErrorType errorType, string errorMessage, bool isFatal)
    {
        reportError(lineNumber, columnIndex, errorType, errorMessage, "", isFatal);
    }
    
    void ErrorReporter::reportError(unsigned int lineNumber, unsigned int columnIndex, ErrorType errorType, std::string errorMessage, std::string additionalInformation, bool isFatal)
    {
        printErrorHeader(errorType, lineNumber, columnIndex, errorMessage);
        
        if (columnIndex != COL_INDEX_NA) {
            std::string line = getLineOfInput(lineNumber - 1);

            printLineWithArrow(line, columnIndex);
        } else if (lineNumber != LINE_NUMBER_NA) {
            std::string line = getLineOfInput(lineNumber - 1);
            cerr << "\n  " << line << "\n";
        }
        
        if (!additionalInformation.empty()) {
            cerr << "\n\n" << Utilities::stringWithLineIndentation(additionalInformation, 2);
        }
        
        cerr << endl;
        
        m_hasReportedErrors = true;
        
        if (isFatal)
            exit(EXIT_FAILURE);
    }
    
    void ErrorReporter::reportError(unsigned int lineNumber, Range range, ErrorType errorType, std::string errorMessage, std::string additionalInformation, bool isFatal)
    {
        printErrorHeader(errorType, lineNumber, range.getLocation(), errorMessage);
        
        std::string line = getLineOfInput(lineNumber - 1);
        printLineWithUnderline(line, range);
        
        if (!additionalInformation.empty()) {
            cerr << "\n\n" << Utilities::stringWithLineIndentation(additionalInformation, 2);
        }
        
        cerr << endl;
        
        m_hasReportedErrors = true;
        
        if (isFatal)
            exit(EXIT_FAILURE);
    }
    
    void ErrorReporter::printLineWithArrow(std::string line, unsigned int arrowPosition)
    {
        cerr << "\n  " << line << "\n  ";
        cerr << setw(arrowPosition+1) << "^";
    }
    
    void ErrorReporter::printLineWithUnderline(std::string line, Range range)
    {
        cerr << "\n  " << line << "\n  ";
        
        for (unsigned int i = 0; i < line.size(); ++i) {
            if (i < range.getLocation() || i > range.getLocation() + range.getLength()) {
                cerr << " ";
            }
            else if (i <= range.getLocation() + range.getLength()) {
                cerr << "~";
            }
        }
    }
    
    void ErrorReporter::printErrorHeader(MAlice::ErrorType errorType, unsigned int lineNumber, unsigned int columnIndex, std::string errorMessage)
    {
        switch(errorType)
        {
            case ErrorType::Lexical:
                cerr << "Lexical error";
                break;
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
    
    bool ErrorReporter::hasReportedErrors()
    {
        return m_hasReportedErrors;
    }
    
}; // namespace ErrorReporter