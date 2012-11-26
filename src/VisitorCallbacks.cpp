#include <string>
#include <sstream>
#include <list>
#include <typeinfo>


#include "VisitorCallbacks.h"
#include "ArrayEntity.h"
#include "Entity.h"
#include "FunctionEntity.h"
#include "ProcedureEntity.h"
#include "SemanticChecks.h"
#include "VariableEntity.h"
#include "Utilities.h"

namespace MAlice {

    // Statements
    bool visitNullStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        bool isArray = false;
        ASTNode lvalueNode = Utilities::getChildNodeAtIndex(node,0);
        ASTNode rvalueNode = Utilities::getChildNodeAtIndex(node,1);
        std::string lvalueIdentifier = Utilities::getNodeText(lvalueNode);

        // if lvalue is an array, iterate down to its child identifier. 
        if (Utilities::getNodeType(lvalueNode)==ARRAYSUBSCRIPT)
        {
            // Check this has children and isn't just referenced directly.
            int numChildren = Utilities::getNumberOfChildNodes(lvalueNode);
            lvalueNode = Utilities::getChildNodeAtIndex(lvalueNode,0);
            lvalueIdentifier = Utilities::getNodeText(lvalueNode);
            if (numChildren == 0)
            {
                
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(lvalueNode),
                                                         Utilities::getNodeColumnIndex(lvalueNode),
                                                         ErrorType::Semantic,
                                                         "Array: '" + lvalueIdentifier + "' can't be referenced directly for assignment",
                                                         false);
            }
            isArray = true;
        }

        // TODO: Check these are not null. otherwise fatal error


        Entity *lvalueEntity = NULL;
        
        // Check lvalue exists on the symboltable
        if (ctx->isSymbolInScope(lvalueIdentifier, &lvalueEntity))
        {
            MAliceEntityType entityType = Utilities::getTypeOfEntity(lvalueEntity);
            
            switch(entityType) {
                case  MAliceEntityTypeVariable:
                    break;
                case MAliceEntityTypeArray:
                    if (isArray)
                    {
                        break;
                    }
                default:
                    ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(lvalueNode),
                                                         Utilities::getNodeColumnIndex(lvalueNode),
                                                         ErrorType::Semantic,
                                                         "Identifier: '" + lvalueIdentifier + "' is not a variable.",
                                                         false);
                    return false;
            }
            
            /* TODO: Array Bounds checking..
            if (isArray) {
                ArrayEntity *lvalueTEntity = NULL;
                lvalueTEntity = dynamic_cast<ArrayEntity*>(lvalueEntity);
                if (lvalueTEntity == NULL)
                    throw std::bad_cast();
            }
            else {*/
            //}
            
            VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(lvalueEntity);
            
            // Iterate through expression and return the type, producing errors where relevant, returning the type as rvalue
            checkExpression(rvalueNode , walker, ctx, variableEntity->getType());
        }
        else {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(lvalueNode),
                                                 Utilities::getNodeColumnIndex(lvalueNode),
                                                 ErrorType::Semantic,
                                                 "Cannot find variable declaration for '" + lvalueIdentifier + "'.",
                                                 false);
            return false;
        }



        

        

        // Produce an error where rvalue and lvalue have differing types
        return walker->visitChildren(node, ctx);
    }
    
    bool visitIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) == 1)
        {
            // Check that the child of this node is a number, and it has a child
            return checkExpression(Utilities::getChildNodeAtIndex(node,0), walker, ctx, MAliceTypeNumber);
        }
        return false;
    }
    
    bool visitDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) == 1)
        {
            // Check that the child of this node is a number, and it has a child
            return checkExpression(Utilities::getChildNodeAtIndex(node,0), walker, ctx, MAliceTypeNumber);
        }
        return false;
    }
    
    bool visitIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        bool result = true;
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(node); ++i) {
            ASTNode childNode = Utilities::getChildNodeAtIndex(node, i);
            ANTLR3_UINT32 nodeType = Utilities::getNodeType(childNode);
            
            if (nodeType == EXPRESSION) {
                if (!walker->visitNode(childNode, ctx))
                    result = false;
            }
            else if (nodeType == STATEMENTLIST) {
                ctx->enterScope();
                
                walker->visitNode(childNode, ctx);
                
                ctx->exitScope();
            }
        }
        
        return result;
    }
    
    bool visitInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitPrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        // make sure there is one child
        int numChildren = Utilities::getNumberOfChildNodes(node);
        if (numChildren != 1)
        {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                 Utilities::getNodeColumnIndex(node),
                                                 ErrorType::Internal,
                                                 "PrintNode malformed: '" + Utilities::getNodeTextIncludingChildren(node, ctx, NULL) + "'.",
                                                 false);
            return false;
        }
        ASTNode childNode = Utilities::getChildNodeAtIndex(node, 0);
        // Get the type of the child, if its an expression
        int nodeType = Utilities::getNodeType(childNode);
        if (nodeType == EXPRESSION)
        {
            // Make sure its not undefined
            MAliceType t = getTypeFromExpressionNode(childNode, walker, ctx);
            if (t == MAliceTypeUndefined)
            {
                // Deepest child Node
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                     Utilities::getNodeColumnIndex(node),
                                                     ErrorType::Semantic,
                                                     "Expression: '" + Utilities::getNodeTextIncludingChildren(childNode, ctx, NULL) + "' is not a valid print statement.",
                                                     false);
                return false;
            }
            return true;
        }
        
        
        return walker->visitChildren(node, ctx);
    }
    
    bool visitReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitStatementListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
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

            // array of what?
            ASTNode typeNode = Utilities::getChildNodeAtIndex(identifierNode, 0);
            if (typeNode != NULL)
                type = Utilities::getNodeText(typeNode);
            // length (doesn't matter for validation)

            
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
            MAliceType returnType = MAliceType::MAliceTypeUndefined;
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
            if (!visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(node, functionEntity, walker, ctx))
                return false;
        }
        
        return true;
    }
    
    bool visitProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!checkValidInvocationNode(node, walker, ctx))
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
            if (!visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(node, procedureEntity, walker, ctx))
                return false;
        }
        
        return true;
    }
    
    bool visitVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (ctx->isKeyword(identifier)) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                     Utilities::getNodeColumnIndex(identifierNode),
                                                     ErrorType::Semantic,
                                                     "Cannot declare variable '" + identifier + "' because it is a keyword.",
                                                     true);
                
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