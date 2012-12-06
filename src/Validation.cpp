
#include "Validation.h"

#include "ASTWalker.h"
#include "CompilerContext.h"
#include "ErrorReporter.h"
#include "ErrorFactory.h"
#include "Utilities.h"
#include "SemanticChecks.h"
#include "VariableEntity.h"

namespace MAlice {

    bool Validation::validateAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidAssignmentStatementNode(node, walker, ctx);
    }
    
    bool Validation::validateIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("increment statement"));
            return false;
        }
        
        // Check that the child of this node is a number, and it has a child
        return checkExpression(Utilities::getChildNodeAtIndex(node, 0), true, walker, ctx, MAliceTypeNumber);
    }
    
    bool Validation::validateDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("decrement statement"));
            return false;
        }
        
        // Check that the child of this node is a number, and it has a child
        return checkExpression(Utilities::getChildNodeAtIndex(node,0), true, walker, ctx, MAliceTypeNumber);
    }


    // Separator comment (for git).
    
    bool Validation::validateVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (ctx->isKeyword(identifier)) {
                Error *error = ErrorFactory::createSemanticError("Cannot declare variable '" + identifier + "' because it is a keyword.");
                error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
                
                error->setArrowRanges(Utilities::rangeToSingletonList(
                                                                      Utilities::createRange(Utilities::getNodeLineNumber(identifierNode), Utilities::getNodeColumnIndex(identifierNode))
                                                                      ));
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
            
            checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx);
            
            MAliceType type = Utilities::getTypeFromTypeString(std::string(Utilities::getNodeText(typeNode)));
            
            ctx->addEntityInScope(identifier, new VariableEntity(identifier, Utilities::getNodeLineNumber(identifierNode), type));
            
            // See if a third node exists - if so this is a variable assignment node that we want to visit
            
            ASTNode thirdNode = Utilities::getChildNodeAtIndex(node, 2);
            if (thirdNode != NULL)
                checkExpression(thirdNode, walker, ctx, type);
            
        }
        
        return true;
    }


}
