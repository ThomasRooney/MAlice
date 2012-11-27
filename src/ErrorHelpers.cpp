
#include "ErrorHelpers.h"
#include "CompilerContext.h"
#include "Range.h"
#include "Utilities.h"

namespace MAlice {
  
    void outputInvalidASTError(CompilerContext *ctx, std::string currentValidationType)
    {
        ctx->getErrorReporter()->reportError(ErrorType::Internal,
                                             "Incorrect internal AST representation encountered (from validation of " + currentValidationType + ").",
                                             true);
    }
    
    void outputInvalidLValueError(CompilerContext *ctx, ASTNode node)
    {
        Range errorRange;
        std::string invalidExpression = Utilities::getNodeTextIncludingChildren(node, ctx, &errorRange);
        
        ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                             errorRange,
                                             ErrorType::Semantic,
                                             "'" + invalidExpression + "' is not a valid l-value.",
                                             "",
                                             true);
    }
    
};