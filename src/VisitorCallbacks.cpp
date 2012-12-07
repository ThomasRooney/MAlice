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

    bool visitArbitraryBlockNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
    
        bool result = walker->visitChildren(node, NULL, ctx);

        ctx->exitScope();
        
        return result;
    }

    bool visitArithmeticExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitArrayDeclarationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateArrayDeclarationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitArraySubscriptNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitAssignmentStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateAssignmentStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitBitwiseAndExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitBitwiseOrExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitBitwiseXorExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitBodyNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
        if (!Validation::checkReturnValueForAllExecutionPaths(node, walker, ctx))
            return false;
        
        bool result = walker->visitChildren(node, NULL, ctx);
        
        ctx->exitScope();
        
        return result;
    }
    
    bool visitByReferenceParameterNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitByValueParameterNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitCharacterLiteralNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitDeclarationsNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitDecrementStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateDecrementStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitDivideExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitEqualsExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        bool checkChildrenAreValid;
        ctx->beginExpression();

        checkChildrenAreValid = walker->visitChildren(node, NULL, ctx);

        ctx->endExpression();
        return checkChildrenAreValid;
    }

    bool visitFunctionDeclarationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateFunctionDeclarationNode(node, walker, ctx))
            return false;
        
        // We've already validated that there is an identifier node here, so this won't be NULL.
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        bool hasParams = false;
        
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            hasParams = true;
        ASTNode returnNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
        MAliceType returnType = Utilities::getTypeFromTypeString(Utilities::getNodeText(returnNode));
        
        FunctionEntity *functionEntity = new FunctionEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::list<ParameterEntity>(), returnType);
        ctx->addEntityInScope(identifier, functionEntity);
        ctx->pushFunctionProcedureEntity(functionEntity);
        
        bool result = walker->visitChildren(node, NULL, ctx);
        
        ctx->popFunctionProcedureEntity();
        
        return result;
    }

    bool visitGreaterThanExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitIdentifierNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitIfStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateIfStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitIncrementStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateIncrementStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }    

    bool visitInputStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateInputStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitLessThanExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitLessThanOrEqualExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitLetterTypeNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitLogicalAndExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitLogicalNotExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitLogicalOrExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitMinusExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitModuloExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitMultiplyExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitNotEqualExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitNullStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitNumberLiteralNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitNumberTypeNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitParamsNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *entity = ctx->getCurrentFunctionProcedureEntity();
        std::list<ParameterEntity> parameterList = Utilities::getParameterTypesFromParamsNode(node);
        
        for (auto p = parameterList.begin(); p!=  parameterList.end();p++) {
            if (p->isPassedByReference())
                ctx->addEntityInScope(p->getIdentifier(), new ArrayEntity(p->getIdentifier(), p->getLineNumber(),p->getType(), 1));
            else
                ctx->addEntityInScope(p->getIdentifier(), p->clone());
        }
        
        entity->setParameterListTypes(parameterList);
        return true;
    }

    bool visitPlusExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitProcedureDeclarationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateProcedureDeclarationNode(node, walker, ctx))
            return false;
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ProcedureEntity *procedureEntity = new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::list<ParameterEntity>());
        ctx->addEntityInScope(identifier, procedureEntity);
        ctx->pushFunctionProcedureEntity(procedureEntity);
        
        return walker->visitChildren(node, NULL, ctx);
        
        ctx->popFunctionProcedureEntity();
    }

    bool visitProcFuncInvocationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateProcFuncInvocationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitProgramNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitPrintStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validatePrintStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);

    }

    bool visitReturnStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateReturnStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitSentenceTypeNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitStatementListNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitStringLiteralNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitTildeExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitVariableDeclarationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateVariableDeclarationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitWhileStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateWhileStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }
};