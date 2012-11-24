
#include "ErrorReporter.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <sstream>
#include "limits.h"

#define LINE_NUMBER_NA      UINT_MAX
#define COL_INDEX_NA        UINT_MAX

using namespace std;

namespace MAlice {
    
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
          case ErrorTypeSyntactic:
            cerr << "Syntactic error: ";
            break;
          case ErrorTypeSemantic:
            cerr << "Semantic error: ";
            break;
          case ErrorTypeIO:
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