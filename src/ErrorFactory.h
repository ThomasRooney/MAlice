
#ifndef _MALICEERRORFACTORY
#define _MALICEERRORFACTORY

#include <iostream>
#include <vector>

#include "MAliceParser.h"
#include "Types.h"
#include "Error.h"

namespace MAlice {

    class CompilerContext;
    class Type;
    
    class ErrorFactory {
    public:
        static Error *createWarningError(std::string errorMessage);
        static Error *createInternalError(std::string errorMessage);
        static Error *createLexicalError();
        static Error *createLexicalError(std::string errorMessage);
        static Error *createSyntacticError(std::string errorMessage);
        static Error *createSemanticError(std::string errorMessage);
        static Error *createIOError(std::string errorMessage);
        
        static Error *createInvalidASTError(std::string currentValidationType);
        static Error *createInvalidLValueError(ASTNode node, CompilerContext *ctx);
        static Error *createInvalidOperandTypeError(ASTNode operandNode, Type expectedType, Type actualType, CompilerContext *ctx);
        static Error *createInvalidOperandTypeError(ASTNode operatorNode, ASTNode operandNode, Type actualType, std::vector<Type> expectedTypes, CompilerContext *ctx);
        static Error *createCannotMatchTypesError(ASTNode exprNode, Type expectedType, Type actualType, CompilerContext *ctx);
    }; // class ErrorFactory
    
};

#endif /* #ifndef _MALICEERRORFACTORY */
