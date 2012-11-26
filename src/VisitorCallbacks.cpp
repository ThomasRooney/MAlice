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
        MAliceType type = getTypeFromExpressionNode(node, walker, ctx);
        if (typeConfirm != type)
        {
            // TODO: Walker to left, right of expression, take substr of input to give better error output
            std::string expr = std::string((char*)(node->toStringTree(node)->chars));
            ctx->getErrorReporter()->reportError(
                                        Utilities::getNodeLineNumber(node),
                                        Utilities::getNodeColumnIndex(node),
                                        ErrorType::Semantic,
                                        "Expression: '" + expr + "' does not have the expected type: " + \
                                        Utilities::getNameOfTypeFromMAliceType(typeConfirm) + \
                                        "(\'" + Utilities::getNameOfTypeFromMAliceType(type) + "\' != \'" + \
                                        Utilities::getNameOfTypeFromMAliceType(typeConfirm) + "\')",
                                        false);
            return false;
        }
        return true;
    }

    MAliceType getTypeFromExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        // if we're a root node, return the type
        int numChildren = Utilities::getNumberOfChildNodes(node);
        
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
                    switch (entityType) {
                        case  MAliceEntityTypeVariable:
                            lookupVEntity = dynamic_cast<VariableEntity*>(lookupEntity);
                            return lookupVEntity->getType();
                            break;
                        default:
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                                 Utilities::getNodeColumnIndex(node),
                                                                 ErrorType::Semantic,
                                                                 "Identifier: '" + info + "' does not have a type.",
                                                                 false);
                            return MAliceTypeUndefined;
                    }
                }
                else 
                {
                    ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                         Utilities::getNodeColumnIndex(node),
                                                         ErrorType::Semantic,
                                                         "Identifier: '" + info + "' is not in scope.",
                                                         false);
                    return MAliceTypeUndefined;
                }
            }
            else 
            {
                return Utilities::getTypeFromNodeType(type);
            }
        }
        else
        {
            for (int i = 0; i < numChildren; ++i)
            {
                ASTNode childNode = Utilities::getChildNodeAtIndex(node,i);               
                std::string binOperator;
                int numChildrenOfChild;
                MAliceType firstChildType;
                MAliceType secondChildType;
                bool valid;
                // if childNode is an invocationnode, check symbol exists and arguments are correct
                switch (Utilities::getNodeType(childNode))
                {
                    case INVOCATION:
                        return getReturnTypeAndCheckIsValidInvocation(childNode, walker, ctx);
                        break;
                    case EQUALS:
                    case NOTEQUAL:
                    case LESSTHAN:
                    case LESSTHANEQUAL:
                    case GREATERTHAN:
                    case GREATERTHANEQUAL:
                        // Check there are two children
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(childNode);
                        binOperator = Utilities::getNodeText(childNode);
                        if (numChildrenOfChild != 2)
                        {
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(childNode),
                                                                    Utilities::getNodeColumnIndex(childNode),
                                                                    ErrorType::Semantic,
                                                                    "Boolean Operator: '" +\
                                                                    binOperator +\
                                                                    "' must have two children.",
                                                                    false);
                            return MAliceTypeUndefined;
                        }
                        // Check the type of the first child
                        firstChildType = getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(childNode, 0), walker, ctx);
                        // Check the type of the second child is the same as the first child
                        secondChildType = getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(childNode, 0),walker,ctx);
                        if (firstChildType != secondChildType)
                        {
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(childNode),
                                                                    Utilities::getNodeColumnIndex(childNode),
                                                                    ErrorType::Semantic,
                                                                    "Boolean Operator: '" +\
                                                                    binOperator +\
                                                                    "' must have two children.",
                                                                    false);
                            return MAliceTypeUndefined;
                        }
                        return MAliceTypeBoolean;
                        break;
                    case LOGICALAND:
                    case LOGICALOR:
                        // Check there are two children
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(childNode);
                        binOperator = Utilities::getNodeText(childNode);
                        if (numChildrenOfChild != 2)
                        {
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(childNode),
                                                                    Utilities::getNodeColumnIndex(childNode),
                                                                    ErrorType::Semantic,
                                                                    "Boolean Operator: '" +\
                                                                    binOperator +\
                                                                    "' must have children of the same type.",
                                                                    false);
                            return MAliceTypeUndefined;
                        }
                        // Check the type of the first child is a bool
                        valid = checkExpression(Utilities::getChildNodeAtIndex(childNode, 0), walker, ctx, MAliceTypeBoolean);
                        // Check the type of the second child is the same as the first child
                        valid = checkExpression(Utilities::getChildNodeAtIndex(childNode, 1),walker,ctx, MAliceTypeBoolean) && valid ;
                        if (!valid)
                        {
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(childNode),
                                                                    Utilities::getNodeColumnIndex(childNode),
                                                                    ErrorType::Semantic,
                                                                    "Boolean Operator: '" +\
                                                                    binOperator +\
                                                                    "' must have boolean children.",
                                                                    false);
                            return MAliceTypeUndefined;
                        }
                        return MAliceTypeBoolean;
                        break;
                    default:
                    {
                        std::string ident = Utilities::getNodeText(childNode);
                        int type = Utilities::getNodeType(childNode);
                        return getTypeFromExpressionNode(childNode, walker, ctx);
                        break;
                    }
                }
            }
        }
        return MAliceTypeUndefined;
    }

    MAliceType getReturnTypeAndCheckIsValidInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
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
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identNode),
                                                     Utilities::getNodeColumnIndex(identNode),
                                                     ErrorType::Semantic,
                                                     "Procedure '" + ident + "' has no return type. Use a function",
                                                     false);
                
                return MAliceTypeUndefined;
            }
            
            if (entityType != MAliceEntityTypeFunction) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identNode),
                                                     Utilities::getNodeColumnIndex(identNode),
                                                     ErrorType::Internal,
                                                     "Malformed Symbol Table: invocation node refers to a non-function/procedure node!",
                                                     true);
                
                return MAliceTypeUndefined;
            }
            
            FunctionEntity *functionEntity = dynamic_cast<FunctionEntity *>(entityBuffer);

            return functionEntity->getReturnType();            
        }
        else
        {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identNode),
                                                 Utilities::getNodeColumnIndex(identNode),
                                                 ErrorType::Semantic,
                                                 "Function: '" + ident + "' is not in scope.",
                                                 false);
            
        }
        return MAliceTypeUndefined;
    }
    
    bool checkIsValidInvocationAndReturnType(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx, MAliceType type)
    {
        MAliceType returnType = getReturnTypeAndCheckIsValidInvocation(invocationNode, walker, ctx);
        ASTNode identNode = Utilities::getChildNodeAtIndex(invocationNode,0);
        // does childNode return the right type, and is it in scope
        std::string ident = Utilities::getNodeText(identNode);
        if (returnType != type)
        {
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