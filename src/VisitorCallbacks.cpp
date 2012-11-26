#include <string>
#include <sstream>
#include <list>
#include <typeinfo>


#include "VisitorCallbacks.h"
#include "ArrayEntity.h"
#include "Entity.h"
#include "FunctionEntity.h"
#include "ProcedureEntity.h"
#include "VariableEntity.h"
#include "Utilities.h"

#define SSTR( x ) dynamic_cast< std::ostringstream & > \
        ((( std::ostringstream() << std::dec << x ) ).str())

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

        // if lvalue is an array, iterate down to its child identifier. 
        if (Utilities::getNodeType(lvalueNode)==ARRAYSUBSCRIPT)
        {
            lvalueNode = Utilities::getChildNodeAtIndex(lvalueNode,0);
            isArray = true;
        }

        // TODO: Check these are not null. otherwise fatal error
        std::string lvalueIdentifier = Utilities::getNodeText(lvalueNode);

        Entity *lvalueEntity = NULL;
        
        // Check lvalue exists on the symboltable
        if (ctx->isSymbolInScope(lvalueIdentifier, &lvalueEntity))
        {
            VariableEntity *lvalueVEntity = NULL;
            MAliceEntityType entityType = Utilities::getTypeOfEntity(lvalueEntity);
            
            if (entityType != MAliceEntityTypeVariable) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(lvalueNode),
                                                     Utilities::getNodeColumnIndex(lvalueNode),
                                                     ErrorType::Semantic,
                                                     "Identifier: '" + lvalueIdentifier + "' is not a variable!",
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
            
            // Iterate through expression and return the type, producing errors where relevant, returning the type as rvalue
            checkExpression(rvalueNode , walker, ctx, lvalueVEntity->getType());
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
        return walker->visitChildren(node, ctx);
    }
    
    bool visitDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
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
            
            
            ctx->addEntityInScope(identifier, new VariableEntity(identifier, Utilities::getNodeLineNumber(node), Utilities::getTypeFromTypeString(type)));
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
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode == NULL)
            return false;
        
        std::string identifier(Utilities::getNodeText(identifierNode));
        
        Entity *entityForIdentifier = NULL;
        if (!ctx->isSymbolInScope(identifier, &entityForIdentifier)) {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                 Utilities::getNodeColumnIndex(identifierNode),
                                                 ErrorType::Semantic,
                                                 "Cannot find procedure or function declaration for '" + identifier + "'.",
                                                 true);
            
            return false;
        }
        
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


    // Helper methods
    bool checkSymbolNotInCurrentScopeOrOutputError(std::string identifier, ASTNode node, CompilerContext *ctx)
    {
        Entity *existingEntity = NULL;
        
        if (ctx->isSymbolInCurrentScope(identifier, &existingEntity)) {
            std::string errorMessage;
            errorMessage = "'" + identifier + "' has already been declared in the current scope.";
            std::stringstream additionalInformation;
            additionalInformation << "Symbol '" << identifier  << "' is previously declared as a " << existingEntity->humanReadableName() << " on line " << existingEntity->getLineNumber() << ".";
            
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                 Utilities::getNodeColumnIndex(node),
                                                 ErrorType::Semantic,
                                                 errorMessage,
                                                 additionalInformation.str(),
                                                 true);
            
            return false;
        }
        
        return true;
    }
    
    std::list<ParameterEntity> getParameterTypesFromParamsNode(ASTNode paramsNode)
    {
        std::list<ParameterEntity> parameterTypes;
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(paramsNode); ++i) {
            ASTNode childNode = Utilities::getChildNodeAtIndex(paramsNode, i);
            bool passedByReference = (Utilities::getNodeType(childNode) == BYREFERENCE);
            
            ASTNode passTypeNode = Utilities::getChildNodeAtIndex(childNode, 1);
            std::string identifier = Utilities::getNodeText(passTypeNode);
            int lineNumber = Utilities::getNodeLineNumber(passTypeNode);
            ASTNode typeNode = Utilities::getChildNodeAtIndex(childNode, 0);
            
            std::string typeString = Utilities::getNodeText(typeNode);
            MAliceType t = (Utilities::getTypeFromTypeString(typeString));
            ParameterEntity p = ParameterEntity(identifier, lineNumber, t, passedByReference);

            parameterTypes.push_back(p);
        }
        
        return parameterTypes;
    }

    bool visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(ASTNode node, FunctionProcedureEntity *entity, ASTWalker *walker, CompilerContext *ctx)
    {
        std::list<ParameterEntity> parameterList;
        ctx->enterScope();
        
        // Loop through the rest of the child nodes
        for (unsigned int i = 1; i < Utilities::getNumberOfChildNodes(node); ++i) {
            ASTNode childNode = Utilities::getChildNodeAtIndex(node, i);            
                        
            switch (Utilities::getNodeType(childNode))
            {
                case PARAMS:
                    parameterList = getParameterTypesFromParamsNode(childNode);
                    entity->setParameterListTypes(parameterList);
                    
                    for (auto p = parameterList.begin(); p!=  parameterList.end();p++) {
                        ctx->addEntityInScope(p->getIdentifier(), p->clone());
                    }
                    break;
                case BODY:
                    walker->visitNode(childNode, ctx);
                    break;
                default:
                    break;
            }
        }
        
        ctx->exitScope();
        
        return true;
    }
    
    bool checkExpression(ASTNode node, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm)
    {
        int numChildren = Utilities::getNumberOfChildNodes(node);
        bool typeCheck = true;
        
        // Are we at a bottom node yet?
        if (numChildren == 0)
        {
            // If so, check type
            std::string info = Utilities::getNodeText(node);
            int type = Utilities::getNodeType(node);
            MAliceType maType;
            
            if (type == IDENTIFIER)
            {
                Entity * lookupEntity;
                if (ctx->isSymbolInScope(info, &lookupEntity))
                {
                    VariableEntity *lookupVEntity = NULL;
                    MAliceEntityType entityType = Utilities::getTypeOfEntity(lookupEntity);
                    
                    //TODO: make error more expressive.
                    if (entityType != MAliceEntityTypeVariable) {
                        ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                             Utilities::getNodeColumnIndex(node),
                                                             ErrorType::Semantic,
                                                             "Identifier: '" + info + "' is not a variable.",
                                                             false);
                        return false;
                    }
                    
                    lookupVEntity = dynamic_cast<VariableEntity*>(lookupEntity);
                    maType = lookupVEntity->getType();
                    
                }
                else {
                    ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                         Utilities::getNodeColumnIndex(node),
                                                         ErrorType::Semantic,
                                                         "Identifier: '" + info + "' is not in scope.",
                                                         false);
                    return false;
                }
            }
            else {
                maType = Utilities::getTypeFromNodeType(type);
            }
            
            typeCheck = maType == typeConfirm;
            if (!typeCheck)
            {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                     Utilities::getNodeColumnIndex(node),
                                                     ErrorType::Semantic,
                                                     "Expression: '" + info + "' is not of expected type: (\'" \
                                                     + Utilities::getNameOfTypeFromMAliceType(typeConfirm) + "\' != \'" +
                                                     Utilities::getNameOfTypeFromMAliceType(maType) + "\')",
                                                     false);
                
            }
            return typeCheck;
        }
        else
        {
            for (int i = 0; i < numChildren; ++i) {
                ASTNode childNode = Utilities::getChildNodeAtIndex(node,numChildren-1);
                
                // if childNode is an invocationnode, check symbol exists and arguments are correcgt and typecheck
                if (Utilities::getNodeType(childNode) == INVOCATION)
                {
                    typeCheck = checkIsValidInvocationAndReturnType(childNode, walker, ctx, typeConfirm);
                }
                else {
                    typeCheck = checkExpression(childNode, walker, ctx, typeConfirm) && typeCheck;
                }
            }
        }
        return typeCheck;
    }
    
    bool checkIsValidInvocationAndReturnType(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx, MAliceType type)
    {
        ASTNode identNode = Utilities::getChildNodeAtIndex(invocationNode,0);
        
        // does childNode return the right type, and is it in scope
        std::string ident = Utilities::getNodeText(identNode);
        Entity *entityBuffer = NULL;
        
        // Get the entity referred to by the ident, error if its not in scope
        if (ctx->isSymbolInScope(ident, &entityBuffer))
        {
            MAliceEntityType entityType = Utilities::getTypeOfEntity(entityBuffer);
            
            // Ensure it isn't a procedure
            if (entityType == MAliceEntityTypeProcedure) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(invocationNode),
                                                     Utilities::getNodeColumnIndex(invocationNode),
                                                     ErrorType::Semantic,
                                                     "Procedure '" + ident + "' has no return type. Use a function",
                                                     false);
                
                return false;
            }
            
            if (entityType != MAliceEntityTypeFunction) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(invocationNode),
                                                     Utilities::getNodeColumnIndex(invocationNode),
                                                     ErrorType::Internal,
                                                     "Malformed Symbol Table: invocation node refers to a non-function/procedure node!",
                                                     true);
                
                return false;
            }
            
            FunctionEntity *functionEntity = dynamic_cast<FunctionEntity *>(entityBuffer);

            MAliceType returnType = functionEntity->getReturnType();
            if (returnType != type)
                ctx->getErrorReporter()->reportError(
                                                     Utilities::getNodeLineNumber(invocationNode),
                                                     Utilities::getNodeColumnIndex(invocationNode),
                                                     ErrorType::Semantic,
                                                     "Function: '" + ident + "' does not return the expected type: (\'" \
                                                     + Utilities::getNameOfTypeFromMAliceType(type) + "\' != \'" +
                                                     Utilities::getNameOfTypeFromMAliceType(returnType) + "\')",
                                                     false);
            
            return false;
        }
        
        return true;
    }
};