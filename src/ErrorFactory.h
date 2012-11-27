
#ifndef _MALICEERRORFACTORY
#define _MALICEERRORFACTORY

#include <iostream>

#include "MAliceParser.h"
#include "Types.h"
#include "Error.h"

namespace MAlice {

    class CompilerContext;
    
    class ErrorFactory {
    public:
        static Error *createInternalError(std::string errorMessage);
        static Error *createLexicalError();
        static Error *createLexicalError(std::string errorMessage);
        static Error *createSyntacticError(std::string errorMessage);
        static Error *createSemanticError(std::string errorMessage);
        static Error *createIOError(std::string errorMessage);
        
        static Error *createInvalidASTError(std::string currentValidationType);
        static Error *createInvalidLValueError(ASTNode node, CompilerContext *ctx);
    }; // class ErrorFactory
    
};

#endif /* #ifndef _MALICEERRORFACTORY */
