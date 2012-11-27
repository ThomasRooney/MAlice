
#include "ErrorReporter.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <iomanip>
#include <sstream>

#include "CompilerContext.h"
#include "Error.h"
#include "ErrorFactory.h"
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

            MAlice::Error *error = MAlice::ErrorFactory::createSyntacticError(errorMessage);
            error->setErrorPosition(new MAlice::ErrorPosition(token->line, token->charPosition));
            
            parserErrorReporter->reportError(error);
        }
            break;
        case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorFactory::createSyntacticError("Mismatched token exception"));
        }
            break;
        case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
        {
            string tokenText = (char*)token->toString(token);
            string errorMessage = "Unrecognised or missing token.";
            
            MAlice::Error *error = MAlice::ErrorFactory::createSyntacticError(errorMessage);
            error->setErrorPosition(new MAlice::ErrorPosition(token->line));
            
            parserErrorReporter->reportError(error);
        }
            break;
        case ANTLR3_MISMATCHED_SET_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorFactory::createSyntacticError("Mismatched set exception"));
        }
            break;
        case ANTLR3_EARLY_EXIT_EXCEPTION:
        {
            string errorMessage;
            
            if (token && token->type == EOF)
                parserErrorReporter->reportError(MAlice::ErrorFactory::createSyntacticError("Unexpected end of input."));
            else {
                string identifier = (char*)token->getText(token)->chars;
                MAlice::Error *error = MAlice::ErrorFactory::createSyntacticError(errorMessage = "Unexpected token '" + identifier + "'.");
                error->setErrorPosition(new MAlice::ErrorPosition(token->line, token->charPosition));
                
                parserErrorReporter->reportError(error);
            }
        }
            break;
        case ANTLR3_FAILED_PREDICATE_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorFactory::createInternalError("Failed predicate error in parser."));
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
            
            MAlice::Error *error = MAlice::ErrorFactory::createSyntacticError(errorMessage);
            
            if (token)
                error->setErrorPosition(new MAlice::ErrorPosition(token->line, token->charPosition));
            
            parserErrorReporter->reportError(error);
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
            MAlice::Error *error = MAlice::ErrorFactory::createLexicalError();
            
            if (token != NULL) {
                string identifier = (char*)token->getText(token)->chars;
                
                error->setErrorMessage("Unexpected token '" + identifier + "'.");
                error->setErrorPosition(new MAlice::ErrorPosition(token->line, token->charPosition));
            } else
                error->setErrorMessage("Unrecognised token");
            
            parserErrorReporter->reportError(error);
        }
            break;
        case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorFactory::createLexicalError("Mismatched token exception"));
        }
            break;
        case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
        {
            MAlice::Error *error = MAlice::ErrorFactory::createLexicalError("Unrecognised or missing token.");
            error->setErrorPosition(new MAlice::ErrorPosition(exception->line, exception->charPositionInLine));
            
            parserErrorReporter->reportError(error);
        }
            break;
        case ANTLR3_MISMATCHED_SET_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorFactory::createLexicalError("Mismatched set exception"));
        }
            break;
        case ANTLR3_EARLY_EXIT_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorFactory::createLexicalError("Early exit exception"));
        }
            break;
        case ANTLR3_FAILED_PREDICATE_EXCEPTION:
        {
            parserErrorReporter->reportError(MAlice::ErrorFactory::createLexicalError("Failed predicate exception"));
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

    void ErrorReporter::reportError(Error *error)
    {
        Range *range = error->getRange();
        ErrorPosition *errorPosition = error->getErrorPosition();
        
        printErrorHeader(error);
        
        if (range && errorPosition) {
            std::string line = getLineOfInput(errorPosition->getLineNumber() - 1);
            printLineWithUnderline(line, error->getRange());
        }
        else if (errorPosition) {
            if (errorPosition->hasColumnIndex())
                printLineWithArrow(errorPosition);
            else {
                std::string line = getLineOfInput(errorPosition->getLineNumber() - 1);
                cerr << "\n  " << line << "\n";
            }
        }
        
        if (!error->getAdditionalInformation().empty()) {
            cerr << "\n\n" << Utilities::stringWithLineIndentation(error->getAdditionalInformation(), 2);
        }
        
        cerr << endl;
        
        m_hasReportedErrors = true;
        
        delete error;
        
        if (error->getType() == ErrorType::Internal)
            exit(EXIT_FAILURE);
    }
    
    void ErrorReporter::printLineWithArrow(ErrorPosition *errorPosition)
    {
        std::string line = getLineOfInput(errorPosition->getLineNumber() - 1);
        
        cerr << "\n  " << line << "\n  ";
        cerr << setw(errorPosition->getColumnIndex()+1) << "^";
    }
    
    void ErrorReporter::printLineWithUnderline(std::string line, Range *range)
    {
        cerr << "\n  " << line << "\n  ";
        
        for (unsigned int i = 0; i < line.size(); ++i) {
            if (i < range->getLocation() || i >= range->getLocation() + range->getLength()) {
                cerr << " ";
            }
            else if (i <= range->getLocation() + range->getLength()) {
                cerr << "~";
            }
        }
    }
    
    void ErrorReporter::printErrorHeader(Error *error)
    {
        switch(error->getType())
        {
            case ErrorType::Warning:
                cerr << "Warning";
                break;
            case ErrorType::Internal:
                cerr << "Internal error";
                break;
            case ErrorType::Lexical:
                cerr << "Lexical error";
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
        
        if (error->getErrorPosition()) {
            cerr << " (";
            cerr << "Line " << error->getErrorPosition()->getLineNumber();
            cerr << ")";
        }
        
        cerr << ": ";
        
        string messageString = error->getErrorMessage().c_str();
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