
#include <sstream>

#include "SemanticChecks.h"

#include "CompilerContext.h"
#include "ArrayEntity.h"
#include "ASTWalker.h"
#include "ErrorFactory.h"
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
            
            Error *error = ErrorFactory::createSemanticError(errorMessage);
            error->setErrorPosition(new ErrorPosition(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node)));
            error->setAdditionalInformation(additionalInformation.str());
            
            ctx->getErrorReporter()->reportError(error);
            
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
            Range *exprRange = NULL;
            std::string exprText = Utilities::getNodeTextIncludingChildren(node, ctx, &exprRange);
            
            std::string errorMessage;
            if (type == MAliceTypeNone)
                errorMessage = "Can't match expected type '" + std::string(Utilities::getNameOfTypeFromMAliceType(typeConfirm)) + "' with procedure invocation.";
            else {
                errorMessage = "Can't match expected type '" + std::string(Utilities::getNameOfTypeFromMAliceType(typeConfirm)) + \
                "' with type '" + std::string(Utilities::getNameOfTypeFromMAliceType(type)) + "' for expression '" + \
                exprText + "'.";
            }
            
            ASTNode firstNonImaginaryNode = Utilities::getFirstNonImaginaryChildNode(node);
            Error *error = ErrorFactory::createSemanticError(errorMessage);
            error->setErrorPosition(new ErrorPosition(Utilities::getNodeLineNumber(firstNonImaginaryNode)));
            error->setRange(exprRange);
            
            ctx->getErrorReporter()->reportError(error);
            
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
                            Error *error = ErrorFactory::createSemanticError("Cannot refer to array '" + info + "' directly.");
                            error->setErrorPosition(Utilities::getErrorPositionFromNode(node));
                            error->setAdditionalInformation("To fix, specify an array element.");
                            ctx->getErrorReporter()->reportError(error);
                            
                            return lookupAEntity->getType();
                            
                        }
                            break;
                        default:
                            Error *error = ErrorFactory::createSemanticError("Identifier '" + info + "' does not have a type.");
                            error->setErrorPosition(Utilities::getErrorPositionFromNode(node));
                            ctx->getErrorReporter()->reportError(error);

                            return MAliceTypeNone;
                    }
                }
                else
                {
                    Error *error = ErrorFactory::createSemanticError("Identifier: '" + info + "' is not in scope.");
                    error->setErrorPosition(Utilities::getErrorPositionFromNode(node));
                    
                    ctx->getErrorReporter()->reportError(error);
                    
                    return MAliceTypeNone;
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
                    {
                        if (!checkSymbolForInvocationIsValidOrOutputError(node, walker, ctx))
                            return MAliceTypeNone;
                        
                        return getReturnTypeForInvocation(node, walker, ctx);
                    }
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
                            Error *error = ErrorFactory::createSemanticError("Boolean Operator: '" + binOperator + "' must have two children.");
                            error->setErrorPosition(Utilities::getErrorPositionFromNode(node));
                            
                            ctx->getErrorReporter()->reportError(error);
                            
                            return MAliceTypeBoolean;
                        }
                        // Check the type of the first child
                        firstChildType = getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0), walker, ctx);
                        // Check the type of the second child is the same as the first child
                        secondChildType = getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0),walker,ctx);
                        if (firstChildType != secondChildType)
                        {
                            Error *error = ErrorFactory::createSemanticError("Boolean Operator: '" + binOperator + "' must have two children.");
                            error->setErrorPosition(Utilities::getErrorPositionFromNode(node));
                            
                            ctx->getErrorReporter()->reportError(error);
                        }
                        return MAliceTypeBoolean;
                        break;
                    case LOGICALAND:
                    case LOGICALOR:
                        // Check there are two children
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(node);
                        binOperator = Utilities::getNodeText(node);
                        if (numChildrenOfChild != 2)
                        {
                            Error *error = ErrorFactory::createSemanticError("Boolean Operator: '" + binOperator + "' must have children of the same type.");
                            error->setErrorPosition(Utilities::getErrorPositionFromNode(node));
                            
                            ctx->getErrorReporter()->reportError(error);

                            return MAliceTypeBoolean;
                        }
                        // Check the type of the first child is a bool
                        valid = checkExpression(Utilities::getChildNodeAtIndex(node, 0), walker, ctx, MAliceTypeBoolean);
                        // Check the type of the second child is the same as the first child
                        valid = checkExpression(Utilities::getChildNodeAtIndex(node, 1),walker,ctx, MAliceTypeBoolean) && valid ;
                        if (!valid)
                        {
                            Error *error = ErrorFactory::createSemanticError("Boolean Operator: '" + binOperator + "' must have boolean children.");
                            error->setErrorPosition(Utilities::getErrorPositionFromNode(node));
                            
                            ctx->getErrorReporter()->reportError(error);
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
                            Error *error = ErrorFactory::createInternalError("Malformed Array '" + binOperator + "'.");
                            error->setErrorPosition(Utilities::getErrorPositionFromNode(childNode));
                            
                            ctx->getErrorReporter()->reportError(error);
                            
                            return MAliceTypeNone;
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
                                {
                                    Error *error = ErrorFactory::createInternalError("Malformed Array Entity");
                                    error->setErrorPosition(Utilities::getErrorPositionFromNode(node));
                                    
                                    ctx->getErrorReporter()->reportError(error);
                                }
                            }
                        }
                        break;
                        //
                    case BANG:
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(node);
                        if (numChildrenOfChild != 1)
                            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("Bang (!)"));
                        
                        checkExpression(Utilities::getChildNodeAtIndex(node, 0), walker, ctx, MAliceTypeBoolean);
                        return MAliceTypeBoolean;

                        break;
                    case MINUS:
                    case PLUS:
                    case TILDE:
                        // Case where these are used as unary operators just because
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(node);
                        if (numChildrenOfChild == 1)
                        {
                            checkExpression(Utilities::getChildNodeAtIndex(node, 0), walker, ctx, MAliceTypeNumber);
                            return MAliceTypeNumber;
                            break;
                        }
                    

                    case BITWISEXOR:
                    case BITWISEOR:
                    case BITWISEAND:

                    case MODULO:
                    case MULTIPLY:
                        numChildrenOfChild = Utilities::getNumberOfChildNodes(node);
                        // make sure this has two children.
                        if (numChildrenOfChild != 2)
                        {
                            Error *error = ErrorFactory::createInternalError("Malformed Expression '" + binOperator + "'.");
                            error->setErrorPosition(Utilities::getErrorPositionFromNode(childNode));
                            
                            ctx->getErrorReporter()->reportError(error);
                            
                            return MAliceTypeNone;
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
        return MAliceTypeNone;
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
            ctx->getErrorReporter()->reportError(ErrorFactory::createInternalError("Couldn't correctly validate function invocation return type."));
            
            return false;
        }
        
        return functionEntity->getReturnType() == expectedType;
    }
    
    FunctionProcedureEntity *getFunctionProcedureEntityForInvocationNode(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        std::string identifier = getFunctionProcedureInvocationIdentifier(invocationNode, walker, ctx);
        Entity *entity = NULL;
        
        if (!ctx->isSymbolInScope(identifier, &entity))
            return NULL;
        
        MAliceEntityType entityType = Utilities::getTypeOfEntity(entity);
        
        if (entityType != MAliceEntityTypeProcedure && entityType != MAliceEntityTypeFunction)
            return NULL;
        
        return dynamic_cast<FunctionProcedureEntity*>(entity);
    }
    
    std::string getFunctionProcedureInvocationIdentifier(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(invocationNode) == 0) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("function or procedure invocation"));
            return NULL;
        }
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
        return Utilities::getNodeText(identifierNode);
    }
    
    bool checkSymbolForInvocationIsValidOrOutputError(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(invocationNode) == 0) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("function or procedure invocation"));
            return false;
        }
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
        
        std::string identifier = Utilities::getNodeText(identifierNode);
        Entity *entity = NULL;
        
        if (!ctx->isSymbolInScope(identifier, &entity)) {
            Error *error = ErrorFactory::createSemanticError("Cannot find declaration for invocation of function or procedure '" + identifier + "'.");
            error->setErrorPosition(Utilities::getErrorPositionFromNode(identifierNode));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        return true;
    }
    
    MAliceType getReturnTypeForInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        if (!funcProcEntity)
            return MAliceTypeNone;
        
        MAliceEntityType entityType = Utilities::getTypeOfEntity(funcProcEntity);
        
        // Check for a valid procedure invocation
        if (entityType == MAliceEntityTypeProcedure)
            return MAliceTypeNone;
        
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(funcProcEntity);
        
        return functionEntity->getReturnType();
    }
    
    bool checkNumberOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        if (!funcProcEntity)
            return false;
        
        // Subtract one to take account for the identifier node
        unsigned int numberOfInvokedParameters = Utilities::getNumberOfChildNodes(invocationNode) - 1;
        
        if (funcProcEntity->getParameterListTypes().size() != numberOfInvokedParameters)
            return false;
        
        return true;
    }
    
    bool checkTypesOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        if (!funcProcEntity)
            return false;
        
        std::list<ParameterEntity> parameterTypes = funcProcEntity->getParameterListTypes();
        
        unsigned int i = 0;
        for (std::list<ParameterEntity>::iterator it = parameterTypes.begin(); it != parameterTypes.end(); ++it) {
            ParameterEntity paramEntity = *it;
            ASTNode expressionNode = Utilities::getChildNodeAtIndex(invocationNode, i+1);
            if (!expressionNode)
                return false;
            
            MAliceType expressionType = getTypeFromExpressionNode(expressionNode, walker, ctx);
            if (expressionType != paramEntity.getType()) {
                Range *expressionRange = NULL;
                Utilities::getNodeTextIncludingChildren(expressionNode, ctx, &expressionRange);
                
                std::string funcProcIdentifier = getFunctionProcedureInvocationIdentifier(invocationNode, walker, ctx);
                std::string expressionTypeString(Utilities::getNameOfTypeFromMAliceType(expressionType));
                std::string expectedTypeString(Utilities::getNameOfTypeFromMAliceType(paramEntity.getType()));
                
                std::ostringstream stream;
                stream << i+1;
                
                Error *error = ErrorFactory::createSemanticError("Cannot match types of argument #" + stream.str() + " in invocation of '" + funcProcIdentifier + "' Expected argument of type '" + expectedTypeString + "' but found '" + expressionTypeString + "'.");
                error->setErrorPosition(new ErrorPosition(Utilities::getNodeLineNumber(expressionNode)));
                error->setRange(expressionRange);
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
            
            ++i;
        }
        
        return true;
    }
    
    bool checkValidAssignmentStatementNode(ASTNode assignmentStatementNode, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(assignmentStatementNode) < 2) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("assignment statement"));
            return false;
        }
        
        ASTNode lvalueNode = Utilities::getChildNodeAtIndex(assignmentStatementNode, 0);
        ASTNode rvalueNode = Utilities::getChildNodeAtIndex(assignmentStatementNode, 1);
        
        bool isLValueArray = false;
        
        ASTNode parentNode = lvalueNode;
        
        if (Utilities::getNodeType(lvalueNode) != EXPRESSION) {
                ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("assignment statement"));
            return false;
        }
        
        lvalueNode = Utilities::getChildNodeAtIndex(lvalueNode, 0);
        ANTLR3_UINT32 topLevelExpressionNodeType = Utilities::getNodeType(lvalueNode);
        
        std::string lvalueIdentifier = Utilities::getNodeText(lvalueNode);
        
        Range *invalidExpressionRange = NULL;
        std::string invalidExpression = Utilities::getNodeTextIncludingChildren(lvalueNode, ctx, &invalidExpressionRange);
        
        if (topLevelExpressionNodeType != ARRAYSUBSCRIPT && topLevelExpressionNodeType != IDENTIFIER) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidLValueError(lvalueNode, ctx));
            return false;
        }
        
        
        // if lvalue is an array, iterate down to its child identifier.
        if (Utilities::getNodeType(lvalueNode) == ARRAYSUBSCRIPT)
        {
            // Check this has children and isn't just referenced directly.
            int numChildren = Utilities::getNumberOfChildNodes(lvalueNode);
            
            if (numChildren == 0) {
                ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("assignment statement"));
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
            Error *error = ErrorFactory::createSemanticError("Cannot find variable declaration for '" + lvalueIdentifier + "'.");
            error->setErrorPosition(Utilities::getErrorPositionFromNode(lvalueNode));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        } else {
            MAliceEntityType symbolTableEntityType = Utilities::getTypeOfEntity(symbolTableEntity);
            
            if (symbolTableEntityType == MAliceEntityTypeArray && !isLValueArray) {
                Error *error = ErrorFactory::createSemanticError("Trying to assign to array '" + lvalueIdentifier + "' directly is invalid.");
                error->setErrorPosition(Utilities::getErrorPositionFromNode(lvalueNode));
                error->setAdditionalInformation("To fix, assign to one of its elements.");
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
            
            if (symbolTableEntityType == MAliceEntityTypeVariable && isLValueArray) {
                Range *errorRange = NULL;
                
                std::string lvalueText = Utilities::getNodeTextIncludingChildren(parentNode, ctx, &errorRange);
                
                Error *error = ErrorFactory::createSemanticError("Cannot assign value to '" + lvalueText + "' as '" + lvalueIdentifier + "' is not an array.");
                error->setErrorPosition(new ErrorPosition(Utilities::getNodeLineNumber(lvalueNode)));
                error->setRange(errorRange);
                
                ctx->getErrorReporter()->reportError(error);
                
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
                    ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("if statement"));
            }
        }
        
        return result;
    }
    
    bool checkValidPrintStatementNode(ASTNode printStatementNode, ASTWalker *walker, CompilerContext *ctx)
    {
        // make sure there is one child
        int numChildren = Utilities::getNumberOfChildNodes(printStatementNode);
        if (numChildren != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("print statement"));
            return false;
        }
        
        ASTNode childNode = Utilities::getChildNodeAtIndex(printStatementNode, 0);
        // Get the type of the child, if its an expression
        int nodeType = Utilities::getNodeType(childNode);
        if (nodeType == EXPRESSION)
        {
            // Make sure its not undefined
            MAliceType t = getTypeFromExpressionNode(childNode, walker, ctx);
            if (t == MAliceTypeNone)
            {
                Error *error = ErrorFactory::createSemanticError("Expression: '" + Utilities::getNodeTextIncludingChildren(childNode, ctx, NULL) + "' is not a valid print statement.");
                error->setErrorPosition(Utilities::getErrorPositionFromNode(printStatementNode));
                
                // Deepest child Node
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
        }
        
        return true;
    }
}
