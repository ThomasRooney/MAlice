
#include <sstream>

#include "SemanticChecks.h"

#include "CompilerContext.h"
#include "ArrayEntity.h"
#include "ASTWalker.h"
#include "ErrorHelpers.h"
#include "Entity.h"
#include "FunctionEntity.h"
#include "FunctionProcedureEntity.h"
#include "ParameterEntity.h"
#include "Utilities.h"

namespace MAlice {
    
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
                        if (p->isPassedByReference())
                        {
                            ctx->addEntityInScope(p->getIdentifier(), new ArrayEntity(p->getIdentifier(), p->getLineNumber(),p->getType(), 1));
                        }
                        else {
                            ctx->addEntityInScope(p->getIdentifier(), p->clone());
                        }
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
            Range exprRange;
            std::string exprText = Utilities::getNodeTextIncludingChildren(node, ctx, &exprRange);
            
            std::string errorMessage;
            if (type == MAliceTypeUndefined)
                errorMessage = "Can't match expected type '" + std::string(Utilities::getNameOfTypeFromMAliceType(typeConfirm)) + "' with procedure invocation.";
            else {
                errorMessage = "Can't match expected type '" + std::string(Utilities::getNameOfTypeFromMAliceType(typeConfirm)) + \
                "' with type '" + std::string(Utilities::getNameOfTypeFromMAliceType(type)) + "' for expression '" + \
                exprText + "'.";
            }
            
            ASTNode firstNonImaginaryNode = Utilities::getFirstNonImaginaryChildNode(node);
            ctx->getErrorReporter()->reportError(
                                                 Utilities::getNodeLineNumber(firstNonImaginaryNode),
                                                 exprRange,
                                                 ErrorType::Semantic,
                                                 errorMessage,
                                                 "",
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
                        case MAliceEntityTypeArray:
                        {
                            // Array has no children, hence this is an error!
                            ArrayEntity *lookupAEntity = dynamic_cast<ArrayEntity*>(lookupEntity);
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                                 Utilities::getNodeColumnIndex(node),
                                                                 ErrorType::Semantic,
                                                                 "Array: '" + info + "' is not valid in this context",
                                                                 false);
                            
                            return lookupAEntity->getType();
                            
                        }
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
                ASTNode nodeBuffer;
                std::string binOperator;
                std::string stringBuffer;
                int numChildrenOfChild;
                Entity * entityBuffer;
                MAliceType firstChildType;
                MAliceType secondChildType;
                bool valid;
                // if childNode is an invocationnode, check symbol exists and arguments are correct
                switch (Utilities::getNodeType(node))
                {
                    case EXPRESSION:
                        return getTypeFromExpressionNode(childNode, walker, ctx);
                        break;
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
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(node);
                        binOperator = Utilities::getNodeText(node);
                        if (numChildrenOfChild != 2)
                        {
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                                 Utilities::getNodeColumnIndex(node),
                                                                 ErrorType::Semantic,
                                                                 "Boolean Operator: '" +\
                                                                 binOperator +\
                                                                 "' must have two children.",
                                                                 false);
                            return MAliceTypeBoolean;
                        }
                        // Check the type of the first child
                        firstChildType = getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0), walker, ctx);
                        // Check the type of the second child is the same as the first child
                        secondChildType = getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0),walker,ctx);
                        if (firstChildType != secondChildType)
                        {
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                                 Utilities::getNodeColumnIndex(node),
                                                                 ErrorType::Semantic,
                                                                 "Boolean Operator: '" +\
                                                                 binOperator +\
                                                                 "' must have two children.",
                                                                 false);
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
                            return MAliceTypeBoolean;
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
                        }
                        return MAliceTypeBoolean;
                        break;
                    case INCREMENTSTATEMENT:
                    case DECREMENTSTATEMENT:
                        checkExpression(childNode, walker, ctx, MAliceTypeNumber);
                        return MAliceTypeNumber;
                        break;
                    case ARRAYSUBSCRIPT:
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(node);
                        // make sure this has two children.
                        if (numChildrenOfChild != 2)
                        {
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(childNode),
                                                                 Utilities::getNodeColumnIndex(childNode),
                                                                 ErrorType::Internal,
                                                                 "Malformed Array '" +\
                                                                 binOperator +\
                                                                 "' ",
                                                                 false);
                            return MAliceTypeUndefined;
                        }
                        nodeBuffer = Utilities::getChildNodeAtIndex(node, 0);
                        stringBuffer = Utilities::getNodeText(nodeBuffer);
                        // check its in scope
                        if (ctx->isSymbolInScope(stringBuffer, &entityBuffer))
                        {
                            
                            MAliceEntityType entityType = Utilities::getTypeOfEntity(entityBuffer);
                            
                            //TODO: make error more expressive.
                            switch (entityType) {
                                case MAliceEntityTypeArray:
                                {
                                    // Array has no children, hence this is an error!
                                    ArrayEntity *lookupAEntity = NULL;
                                    lookupAEntity = dynamic_cast<ArrayEntity*>(entityBuffer);
                                    
                                    return lookupAEntity->getType();
                                }
                                default:
                                    ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                                         Utilities::getNodeColumnIndex(node),
                                                                         ErrorType::Internal,
                                                                         "Malformed Array Entity",
                                                                         false);
                            }
                        }
                        break;
                        //
                    case MINUS:
                    case PLUS:
                    case MODULO:
                    case MULTIPLY:
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(node);
                        // make sure this has two children.
                        if (numChildrenOfChild != 2)
                        {
                            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(childNode),
                                                                 Utilities::getNodeColumnIndex(childNode),
                                                                 ErrorType::Internal,
                                                                 "Malformed Expression '" +\
                                                                 binOperator +\
                                                                 "' ",
                                                                 false);
                            return MAliceTypeUndefined;
                        }
                        checkExpression(Utilities::getChildNodeAtIndex(node, 0), walker, ctx, MAliceTypeNumber);
                        checkExpression(Utilities::getChildNodeAtIndex(node, 1), walker, ctx, MAliceTypeNumber);
                        // Check they return numbers 
                        return MAliceTypeNumber;
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
    
    bool checkValidInvocationNode(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(invocationNode) == 0)
            return false;
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        Entity *entity = NULL;
        
        if (!ctx->isSymbolInScope(identifier, &entity)) {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                 Utilities::getNodeColumnIndex(identifierNode),
                                                 ErrorType::Semantic,
                                                 "Invocation of function or procedure '" + identifier + "' which is not declared in current scope.",
                                                 false);
            
            return false;
        }
        
        MAliceEntityType entityType = Utilities::getTypeOfEntity(entity);
        
        if (entityType != MAliceEntityTypeFunction && entityType != MAliceEntityTypeProcedure) {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                 Utilities::getNodeLineNumber(identifierNode),
                                                 ErrorType::Semantic,
                                                 "Invoked symbol '" + identifier + "' is not a function or procedure.",
                                                 false);
            
            return false;
        }
        
        return true;
    }
    
    bool checkCompatibleFunctionInvocationReturnType(ASTNode invocationNode, MAliceType expectedType, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(invocationNode) == 0)
            return false;
        
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        Entity *entity = NULL;
        
        if (!ctx->isSymbolInScope(identifier, &entity))
            return false;
        
        MAliceEntityType entityType = Utilities::getTypeOfEntity(entity);
        if (entityType != MAliceEntityTypeFunction)
            return false;
        
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(entity);
        if (functionEntity == NULL) {
            ctx->getErrorReporter()->reportError(ErrorType::Internal,
                                                 "Couldn't correctly validate function invocation return type.",
                                                 true);
            
            return false;
        }
        
        return functionEntity->getReturnType() == expectedType;
    }
    
    MAliceType getReturnTypeAndCheckIsValidInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
        
        // does childNode return the right type, and is it in scope
        std::string identifier = Utilities::getNodeText(identifierNode);
        Entity *entity = NULL;
        
        // Get the entity referred to by the ident, error if its not in scope
        if (ctx->isSymbolInScope(identifier, &entity))
        {
            MAliceEntityType entityType = Utilities::getTypeOfEntity(entity);
            
            // Ensure it isn't a procedure
            if (entityType == MAliceEntityTypeProcedure) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                     Utilities::getNodeColumnIndex(identifierNode),
                                                     ErrorType::Semantic,
                                                     "Procedure '" + identifier + "' has no return type.",
                                                     false);
                
                return MAliceTypeUndefined;
            }
            
            if (entityType != MAliceEntityTypeFunction) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                     Utilities::getNodeColumnIndex(identifierNode),
                                                     ErrorType::Internal,
                                                     "Malformed Symbol Table: invocation node refers to a non-function/procedure node!",
                                                     true);
                
                return MAliceTypeUndefined;
            }
            
            FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(entity);
            
            return functionEntity->getReturnType();
        }
        else
        {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                 Utilities::getNodeColumnIndex(identifierNode),
                                                 ErrorType::Semantic,
                                                 "Function: '" + identifier + "' is not in scope.",
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
    
    bool checkValidAssignmentStatementNode(ASTNode assignmentStatementNode, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(assignmentStatementNode) < 2) {
            outputInvalidASTError(ctx, "assignment statement");
            return false;
        }
        
        ASTNode lvalueNode = Utilities::getChildNodeAtIndex(assignmentStatementNode, 0);
        ASTNode rvalueNode = Utilities::getChildNodeAtIndex(assignmentStatementNode, 1);
        
        bool isLValueArray = false;
        
        ASTNode parentNode = lvalueNode;
        
        if (Utilities::getNodeType(lvalueNode) != EXPRESSION) {
            outputInvalidASTError(ctx, "assignment statement");
            return false;
        }
        
        lvalueNode = Utilities::getChildNodeAtIndex(lvalueNode, 0);
        ANTLR3_UINT32 topLevelExpressionNodeType = Utilities::getNodeType(lvalueNode);
        
        std::string lvalueIdentifier = Utilities::getNodeText(lvalueNode);
        
        Range invalidExpressionRange;
        std::string invalidExpression = Utilities::getNodeTextIncludingChildren(lvalueNode, ctx, &invalidExpressionRange);
        
        if (topLevelExpressionNodeType != ARRAYSUBSCRIPT && topLevelExpressionNodeType != IDENTIFIER) {
            outputInvalidLValueError(ctx, lvalueNode);
            return false;
        }
        
        
        // if lvalue is an array, iterate down to its child identifier.
        if (Utilities::getNodeType(lvalueNode) == ARRAYSUBSCRIPT)
        {
            // Check this has children and isn't just referenced directly.
            int numChildren = Utilities::getNumberOfChildNodes(lvalueNode);
            
            if (numChildren == 0) {
                outputInvalidASTError(ctx, "assignment statement");
                return false;
            }
            
            lvalueNode = Utilities::getChildNodeAtIndex(lvalueNode,0);
            lvalueIdentifier = Utilities::getNodeText(lvalueNode);
            isLValueArray = true;
        }
        
        // TODO: Check these are not null. otherwise fatal error
        
        
        Entity *symbolTableEntity = NULL;
        
        // Check lvalue exists on the symbol table
        if (!ctx->isSymbolInScope(lvalueIdentifier, &symbolTableEntity)) {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(lvalueNode),
                                                 Utilities::getNodeColumnIndex(lvalueNode),
                                                 ErrorType::Semantic,
                                                 "Cannot find variable declaration for '" + lvalueIdentifier + "'.",
                                                 false);
            
            return false;
        } else {
            MAliceEntityType symbolTableEntityType = Utilities::getTypeOfEntity(symbolTableEntity);
            
            if (symbolTableEntityType == MAliceEntityTypeArray && !isLValueArray) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(lvalueNode),
                                                     Utilities::getNodeColumnIndex(lvalueNode),
                                                     ErrorType::Semantic,
                                                     "Trying to assign to array '" + lvalueIdentifier + "' directly is invalid.",
                                                     "To fix, assign to one of its elements.",
                                                     false);
                
                return false;
            }
            
            if (symbolTableEntityType == MAliceEntityTypeVariable && isLValueArray) {
                Range errorRange;
                
                std::string lvalueText = Utilities::getNodeTextIncludingChildren(parentNode, ctx, &errorRange);
                
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(lvalueNode),
                                                     errorRange,
                                                     ErrorType::Semantic,
                                                     "Cannot assign value to '" + lvalueText + "' as '" + lvalueIdentifier + "' is not an array.",
                                                     "",
                                                     false);
                
                return false;
            }
            
            VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(symbolTableEntity);
            
            // Iterate through expression and return the type, producing errors where relevant, returning the type as rvalue
            checkExpression(rvalueNode, walker, ctx, variableEntity->getType());
        }
    }
    
    bool checkValidIfStatementNode(ASTNode ifStatementNode, ASTWalker *walker, CompilerContext *ctx)
    {
        bool result = true;
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(ifStatementNode); ++i) {
            ASTNode childNode = Utilities::getChildNodeAtIndex(ifStatementNode, i);
            ANTLR3_UINT32 nodeType = Utilities::getNodeType(childNode);
            
            switch(nodeType)
            {
                case EXPRESSION:
                    if (!walker->visitNode(childNode, ctx))
                        result = false;
                    break;
                case STATEMENTLIST:
                    ctx->enterScope();
                    
                    walker->visitNode(childNode, ctx);
                    
                    ctx->exitScope();
                    break;
                default:
                    outputInvalidASTError(ctx, "if statement");
            }
        }
        
        return result;
    }
    
    bool checkValidPrintStatementNode(ASTNode printStatementNode, ASTWalker *walker, CompilerContext *ctx)
    {
        // make sure there is one child
        int numChildren = Utilities::getNumberOfChildNodes(printStatementNode);
        if (numChildren != 1) {
            outputInvalidASTError(ctx, "print statement");
            return false;
        }
        
        ASTNode childNode = Utilities::getChildNodeAtIndex(printStatementNode, 0);
        // Get the type of the child, if its an expression
        int nodeType = Utilities::getNodeType(childNode);
        if (nodeType == EXPRESSION)
        {
            // Make sure its not undefined
            MAliceType t = getTypeFromExpressionNode(childNode, walker, ctx);
            if (t == MAliceTypeUndefined)
            {
                // Deepest child Node
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(printStatementNode),
                                                     Utilities::getNodeColumnIndex(printStatementNode),
                                                     ErrorType::Semantic,
                                                     "Expression: '" + Utilities::getNodeTextIncludingChildren(childNode, ctx, NULL) + "' is not a valid print statement.",
                                                     false);
                return false;
            }
        }
        
        return true;
    }
}
