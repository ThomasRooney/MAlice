#include <string>
#include <sstream>
#include <list>
#include <typeinfo>


#include "VisitorCallbacks.h"
#include "ArrayEntity.h"
#include "Entity.h"
#include "ErrorFactory.h"
#include "FunctionEntity.h"
#include "ProcedureEntity.h"
#include "SemanticChecks.h"
#include "VariableEntity.h"
#include "Utilities.h"
#include "Validation.h"

namespace MAlice {

    // Statements
    bool visitNullStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidAssignmentStatementNode(node, walker, ctx);
    }
    
    bool visitIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("increment statement"));
            return false;
        }
        
        // Check that the child of this node is a number, and it has a child
        return checkExpression(Utilities::getChildNodeAtIndex(node, 0), true, walker, ctx, MAliceTypeNumber);
    }
    
    bool visitDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("decrement statement"));
            return false;
        }
        
        // Check that the child of this node is a number, and it has a child
        return checkExpression(Utilities::getChildNodeAtIndex(node,0), true, walker, ctx, MAliceTypeNumber);
    }
    
    bool visitIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
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
    
    bool visitInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
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
    
    bool visitPrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidPrintStatementNode(node, walker, ctx);
    }
    
    bool visitReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidReturnStatementNode(node, walker, ctx);
    }
    
    bool visitStatementListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
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
    
    // Expressions
    bool visitLogicalAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitLogicalOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitLogicalNotExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitBitwiseAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitBitwiseOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitBitwiseXorExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitDivideExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitEqualsExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitGreaterThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitIdentifierNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitLessThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitLessThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitMinusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitModuloExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitMultiplyExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitNotEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitPlusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitTildeExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    // Imaginary nodes – used to improve AST structure
    bool visitArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
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
    
    bool visitArraySubscriptNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitBodyNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
        bool result = walker->visitChildren(node, ctx);
        ctx->exitScope();
        
        return result;
    }
    
    bool visitByReferenceParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitByValueParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitDeclarationsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitArithmeticExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateFunctionDeclarationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, ctx);
    }
    
    bool visitProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateProcFuncInvocationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, ctx);
    }
    
    bool visitParamsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateProcedureDeclarationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, ctx);
    }
    
    bool visitVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateVariableDeclarationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, ctx);
    }
    
    // Literals
    bool visitCharacterLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitNumberLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitStringLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    // Types
    bool visitLetterTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitNumberTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitSentenceTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }

};