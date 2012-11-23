
#include "ErrorReporter.h"

#define LINE_NUMBER_NA      UINT_MAX
#define COL_INDEX_NA        UINT_MAX

using namespace std;

namespace MAlice {
    
    ErrorReporter::ErrorReporter(string input)
    {
        m_input = input;
    }
    
    void ErrorReporter::reportError(ErrorType errorType, string errorMessage, bool isFatal)
    {
        reportError(LINE_NUMBER_NA, COL_INDEX_NA, errorType, errorMessage, isFatal);
    }
    
    void ErrorReporter::reportError(unsigned int lineNumber, unsigned int columnIndex, ErrorType errorType, string errorMessage, bool isFatal)
    {
        if (errorType == ErrorTypeSyntactic)
            cerr << "Syntactic error: ";
        else if (errorType == ErrorTypeSemantic)
            cerr << "Semantic error: ";
        
        if (lineNumber != LINE_NUMBER_NA || columnIndex != COL_INDEX_NA) {
            cerr << "(";
            
            if (lineNumber != LINE_NUMBER_NA)
                cerr << "Line " << lineNumber;
            
            if (lineNumber != COL_INDEX_NA)
                cerr << ", Col " << columnIndex;
            
            cerr << ") ";
        }
        
        cerr << errorMessage << endl;
        
        if (isFatal)
            exit(EXIT_FAILURE);
    }
    
}; // namespace ErrorReporter