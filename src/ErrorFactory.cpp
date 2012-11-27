
#include "ErrorFactory.h"

#include "CompilerContext.h"
#include "Error.h"
#include "Range.h"
#include "Utilities.h"

namespace MAlice {
    
    Error *ErrorFactory::createWarningError(std::string errorMessage)
    {
        return new Error(ErrorType::Warning, errorMessage);
    }
    
    Error *ErrorFactory::createInternalError(std::string errorMessage)
    {
        return new Error(ErrorType::Internal, errorMessage);
    }

    Error *ErrorFactory::createLexicalError()
    {
        return new Error(ErrorType::Lexical);
    }
    
    Error *ErrorFactory::createLexicalError(std::string errorMessage)
    {
        return new Error(ErrorType::Lexical, errorMessage);
    }
    
    Error *ErrorFactory::createSyntacticError(std::string errorMessage)
    {
        return new Error(ErrorType::Syntactic, errorMessage);
    }
    
    Error *ErrorFactory::createSemanticError(std::string errorMessage)
    {
        return new Error(ErrorType::Semantic, errorMessage);
    }
    
    Error *ErrorFactory::createIOError(std::string errorMessage)
    {
        return new Error(ErrorType::IO, errorMessage);
    }
  
    Error *ErrorFactory::createInvalidASTError(std::string currentValidationType)
    {
        return createInternalError("Incorrect internal AST representation encountered (from validation of " + currentValidationType + ").");
    }
    
    Error *ErrorFactory::createInvalidLValueError(ASTNode node, CompilerContext *ctx)
    {
        Range *errorRange = NULL;
        std::string invalidExpression = Utilities::getNodeTextIncludingChildren(node, ctx, &errorRange);
        
        Error *error = createSemanticError("'" + invalidExpression + "' is not a valid l-value.");
        error->setErrorPosition(new ErrorPosition(Utilities::getNodeLineNumber(node)));
        error->setRange(errorRange);
        
        return error;
    }
    
};