#include "Validation.h"

#include "ArrayEntity.h"
#include "ASTWalker.h"
#include "CompilerContext.h"
#include "ErrorReporter.h"
#include "ErrorFactory.h"
#include "FunctionEntity.h"
#include "Utilities.h"
#include "ProcedureEntity.h"
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

    bool Validation::validateIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        bool checkTotal = true;
        // If -> (Expression -> Statements)*
        int numChildren = Utilities::getNumberOfChildNodes(node);
        if (numChildren < 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("if Statement"));
            return false;
        }
        //  the expression on node 1 must return a a boolean
        for (int i = 0 ; i < numChildren; i++)
        {
            ASTNode childNode = Utilities::getChildNodeAtIndex(node,i);
            switch(Utilities::getNodeType(childNode))
            {
                case EXPRESSION:
                    checkTotal = checkExpression(childNode, walker, ctx, MAliceTypeBoolean) && checkTotal;
                    break;
                default:
                    continue;
            }
        }
        
        return checkTotal;
    }

    bool Validation::validateInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        // input must be either a number or a letter.
        if (Utilities::getNumberOfChildNodes(node) < 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("input Statement"));
            return false;
        }
        MAliceType t = MAliceTypeNone;
        
        if (!getTypeFromExpressionNode(node, &t, true, walker, ctx, NULL))
            return false;
        
        switch(t)
        {
        case MAliceTypeLetter:
        case MAliceTypeNumber:
            return true;
        default:
            {
                Range *r = NULL;
                std::string text = Utilities::getNodeTextIncludingChildren(node, ctx, &r);
                Error *error = ErrorFactory::createSemanticError("Input can only stream to a Letter or a Number variable.  '" + text + "' is neither.");
                error->setLineNumber(Utilities::getNodeLineNumber(node));
                error->setLineNumber(Utilities::getNodeLineNumber(node));
                error->setUnderlineRanges(Utilities::rangeToSingletonList(r));
            }
        }
        return walker->visitChildren(node, ctx);
    }
    
    bool Validation::validatePrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidPrintStatementNode(node, walker, ctx);
    }
    
    bool Validation::validateReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidReturnStatementNode(node, walker, ctx);
    }

    bool Validation::validateWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        // First child must be an expression
        int numChildren = Utilities::getNumberOfChildNodes(node);
        if (numChildren < 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("while Statement with no conditional"));
            return false;
        }
        //  the expression on node 1 must return a a boolean
        ASTNode exprNode = Utilities::getChildNodeAtIndex(node,0);
        switch(Utilities::getNodeType(exprNode))
        {
            case EXPRESSION:
                return checkExpression(exprNode, walker, ctx, MAliceTypeBoolean);
            default:
                return false;
        }        
        return false;
    }



    // Separator comment (for git).
    
    bool Validation::validateArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            std::string type;
            if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
                return false;
            
            // Number of children should be two
            int numChildren = Utilities::getNumberOfChildNodes(identifierNode);
            
            if (numChildren != 2)
                ctx->getErrorReporter()->reportError(ErrorFactory::createInternalError("Malformed Array Invocation Node"));
            
            // array of what?
            ASTNode typeNode = Utilities::getChildNodeAtIndex(identifierNode, 0);
            if (typeNode != NULL)
                type = Utilities::getNodeText(typeNode);
            // length is a number
            ASTNode exprNode = Utilities::getChildNodeAtIndex(identifierNode, 1);
            checkExpression(exprNode,false,walker,ctx,MAliceTypeNumber);
            
            
            ctx->addEntityInScope(identifier, new ArrayEntity(identifier, Utilities::getNodeLineNumber(node), Utilities::getTypeFromTypeString(type), 1));
        }
        
        return true;
    }
    
    bool Validation::validateFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode == NULL) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("function declaration"));
            return false;
        }
        
        std::string identifier(Utilities::getNodeText(identifierNode));
        
        if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
            return false;
        
        bool hasParams = false;
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        if (Utilities::getNodeType(nodeI1) != PARAMS)
            hasParams = true;
        ASTNode returnNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
        if (returnNode == NULL) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("function declaration"));
            return false;
        }
        
        return true;
    }
    
    bool Validation::validateProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!checkSymbolForInvocationIsValidOrOutputError(node, walker, ctx))
            return false;
        
        // This is being called in the context of a statement, not within an expression.
        if (getReturnTypeForInvocation(node, walker, ctx) != MAliceTypeNone) {
            FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(node, walker, ctx);
            std::string identifier = funcProcEntity->getIdentifier();
            
            Range *range = NULL;
            Utilities::getNodeTextIncludingChildren(node, ctx, &range);
            
            Error *error = ErrorFactory::createWarningError("Unused return value from function '" + identifier + "'.");
            
            ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
            if (identifierNode) {
                error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
                error->setUnderlineRanges(Utilities::rangeToSingletonList(range));
            }
            
            ctx->getErrorReporter()->reportError(error);
        }
        
        if (!checkNumberOfArgumentsForInvocationIsValid(node, walker, ctx))
            return false;
        
        if (!checkTypesOfArgumentsForInvocationIsValid(node, walker, ctx))
            return false;
        
        return true;
    }
    
    bool Validation::validateProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        if (identifierNode == NULL) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("procedure declaration"));
            return false;
        }
        
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
            return false;
        
        return true;
    }
    
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