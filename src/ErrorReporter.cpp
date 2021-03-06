
#include "ErrorReporter.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <iomanip>
#include <sstream>

#include "CompilerContext.h"
#include "Error.h"
#include "ErrorFactory.h"
#include "ErrorPosition.h"
#include "limits.h"
#include "Utilities.h"
#include "MAliceParser.h"
#include "Range.h"
#include "Types.h"

#define LINE_NUMBER_NA      UINT_MAX
#define COL_INDEX_NA        UINT_MAX

#define DECORATE_INDENTATION        2

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
            error->setLineNumber(token->line);
            error->setArrowRanges(MAlice::Utilities::rangeToSingletonList(MAlice::Range::create(token->line, token->charPosition)));
            
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
            string errorMessage = "Unrecognised or missing token.";
            
            MAlice::Error *error = MAlice::ErrorFactory::createSyntacticError(errorMessage);
            if (token) {
                error->setLineNumber(token->line);
                
                if (token->charPosition != UINT_MAX) {
                    MAlice::Range *range = MAlice::Range::create(token->line, token->charPosition);
                    error->setArrowRanges(MAlice::Utilities::rangeToSingletonList(range));
                }
                    
            }
            
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
                error->setLineNumber(token->line);
                error->setArrowRanges(MAlice::Utilities::rangeToSingletonList(MAlice::Range::create(token->line, token->charPosition)));
                
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
            
            if (token) {
                error->setLineNumber(token->line);
                error->setArrowRanges(MAlice::Utilities::rangeToSingletonList(MAlice::Range::create(token->line, token->charPosition)));
            }
            
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
                error->setLineNumber(token->line);
                error->setArrowRanges(MAlice::Utilities::rangeToSingletonList(MAlice::Range::create(token->line, token->charPosition)));
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
            error->setLineNumber(exception->line);
            error->setArrowRanges(MAlice::Utilities::rangeToSingletonList(
                (MAlice::Range::create(exception->line, exception->charPositionInLine))));
            
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
        m_numberOfReportedErrors = 0;
        m_numberOfReportedWarnings = 0;
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
        printErrorHeader(error);
        printDecoratedLines(error);
        
        cerr << "\n";

        if (!error->getAdditionalInformation().empty()) {
            cerr << "\n  " << Utilities::stringWithLineIndentation(error->getAdditionalInformation(), DECORATE_INDENTATION);
        }

        cerr << endl;
        
        if (error->getType() == ErrorType::Warning)
            m_numberOfReportedWarnings++;
        else
            m_numberOfReportedErrors++;
        
        delete error;
        
        if (error->getType() == ErrorType::Internal)
            exit(EXIT_FAILURE);
    }
    
    void ErrorReporter::printDecoratedLines(Error *error)
    {
        if (error->getArrowRanges().size() == 0 && error->getUnderlineRanges().size() == 0) {
            if (error->getLineNumber() > 0)
                cerr << "\n" << "  " << getLineOfInput(error->getLineNumber() - 1);
            return;
        }
        
        unsigned int startLine = UINT_MAX;
        unsigned int endLine = 0;

        getStartAndEndLinesForRanges(error->getArrowRanges(), startLine, endLine, &startLine, &endLine);
        getStartAndEndLinesForRanges(error->getUnderlineRanges(), startLine, endLine, &startLine, &endLine);
        
        for (unsigned int i = startLine; i <= endLine; ++i)
        {
            std::string line = getLineOfInput(i - 1);
            
            cerr << "\n" << "  " << line;
            printDecoratedLine(error, line, i - 1);
        }
    }
    
    void ErrorReporter::printDecoratedLine(Error *error, std::string line, unsigned int lineIndex)
    {
        std::string decoratedLine(line.size() + DECORATE_INDENTATION, ' ');
        
        decorateLineWithRanges(&decoratedLine, lineIndex, error->getArrowRanges(), '^');
        decorateLineWithRanges(&decoratedLine, lineIndex, error->getUnderlineRanges(), '~');
        
        cerr << "\n" << decoratedLine;
    }
    
    void ErrorReporter::decorateLineWithRanges(std::string *decorateLine, unsigned int lineIndex, std::list<Range*> ranges, char decorateCharacter)
    {
        if (!decorateLine)
            return;
        
        for (std::list<Range*>::iterator it = ranges.begin(); it != ranges.end(); ++it) {
            Range *r = *it;
            
            unsigned int startLineNumber = r->getStartPosition().getLineNumber() - 1;
            unsigned int endLineNumber = r->getEndPosition().getLineNumber() - 1;
            
            if (startLineNumber <= lineIndex && endLineNumber >= lineIndex) {
                unsigned int startPosition = (startLineNumber == lineIndex) ? r->getStartPosition().getColumnIndex() : 0;
                unsigned int endPosition = (endLineNumber == lineIndex) ? r->getEndPosition().getColumnIndex() : (unsigned int)(*decorateLine).size() - 1;
                
                if (startPosition > decorateLine->size() || endPosition > decorateLine->size())
                    return;
                
                for (unsigned int i = startPosition; i <= endPosition; ++i) {
                    (*decorateLine)[i + DECORATE_INDENTATION] = decorateCharacter;
                }
            }
        }
    }
    
    void ErrorReporter::getStartAndEndLinesForRanges(std::list<Range*> ranges, unsigned int startLine, unsigned int endLine, unsigned int *outStartLine, unsigned int *outEndLine)
    {
        for (std::list<Range*>::iterator it = ranges.begin(); it != ranges.end(); ++it) {
            Range *r = *it;
            
            unsigned int l1 = r->getStartPosition().getLineNumber();
            unsigned int l2 = r->getEndPosition().getLineNumber();
            
            if (l1 < startLine)
                startLine = l1;
            
            if(l2 > endLine)
                endLine = l2;
        }
        
        if (startLine == UINT_MAX || endLine == 0)
            return;
        
        if (outStartLine)
            *outStartLine = startLine;
        
        if (outEndLine)
            *outEndLine = endLine;
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
        
        if (error->getLineNumber() != UINT_MAX && error->getLineNumber() != 0) {
            cerr << " (";
            cerr << "Line " << error->getLineNumber();
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
        return m_numberOfReportedErrors > 0;
    }
    
    bool ErrorReporter::hasReportedWarnings()
    {
        return m_numberOfReportedWarnings > 0;
    }
    
    unsigned int ErrorReporter::getNumberOfReportedErrors()
    {
        return m_numberOfReportedErrors;
    }
    
    unsigned int ErrorReporter::getNumberOfReportedWarnings()
    {
        return m_numberOfReportedWarnings;
    }
    
}; // namespace ErrorReporter