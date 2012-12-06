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
        if (!Validation::validateAssignmentStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, ctx);
    }
    
    bool visitIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateIncrementStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, ctx);
    }
    
    bool visitDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateDecrementStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, ctx);
    }
    
    bool visitIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateIfStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, ctx);
    }
    
    bool visitInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateInputStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, ctx);
    }
    
    bool visitPrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validatePrintStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, ctx);

    }
    
    bool visitReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateReturnStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, ctx);
    }
    
    bool visitStatementListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateWhileStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, ctx);
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
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
                return false;
            
            // Get the return type
            MAliceType returnType = MAliceType::MAliceTypeNone;
            // 
            bool hasParams = false;
            // get node index 1, if its a parameter node, get params...
            ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
            if (Utilities::getNodeType(nodeI1) == PARAMS)
                hasParams = true;
            ASTNode returnNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
            if (returnNode != NULL)
                returnType = Utilities::getTypeFromTypeString(Utilities::getNodeText(returnNode));

            FunctionEntity *functionEntity = new FunctionEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::list<ParameterEntity>(), returnType);
            if (hasParams)
            {
                std::list<ParameterEntity> parameterList = getParameterTypesFromParamsNode(nodeI1);
                functionEntity->setParameterListTypes(parameterList);
                    
                for (auto p = parameterList.begin(); p!=  parameterList.end();p++) {
                    ctx->addEntityInScope(p->getIdentifier(), p->clone());
                }
            }
            ctx->addEntityInScope(identifier, functionEntity);
            ctx->pushFunctionProcedureEntity(functionEntity);
            
            bool result = visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(node, functionEntity, walker, ctx);
            
            ctx->popFunctionProcedureEntity();
            
            return result;
        }
        
        return true;
    }
    
    bool visitProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
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
    
    bool visitParamsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier = Utilities::getNodeText(identifierNode);
            
            if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
                return false;
            
            ProcedureEntity *procedureEntity = new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::list<ParameterEntity>());
            
            ctx->addEntityInScope(identifier, procedureEntity);
            ctx->pushFunctionProcedureEntity(procedureEntity);
            
            bool result = visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(node, procedureEntity, walker, ctx);
            
            ctx->popFunctionProcedureEntity();
            
            return result;
        }
        
        return true;
    }
    
    bool visitVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateVariableDeclarationNode(node, walker, ctx))
            return false;
        
        return true;
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