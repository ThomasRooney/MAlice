
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
    bool Validation::checkSymbolNotInCurrentScopeOrOutputError(std::string identifier, ASTNode node, CompilerContext *ctx)
    {
        Entity *existingEntity = NULL;
        
        if (ctx->isSymbolInCurrentScope(identifier, &existingEntity)) {
            std::string errorMessage;
            errorMessage = "'" + identifier + "' has already been declared in the current scope.";
            std::stringstream additionalInformation;
            additionalInformation << "Symbol '" << identifier  << "' is previously declared as a " << existingEntity->humanReadableName() << " on line " << existingEntity->getLineNumber() << ".";
            
            Error *error = ErrorFactory::createSemanticError(errorMessage);
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node))));
            error->setAdditionalInformation(additionalInformation.str());
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        return true;
    }
    
    bool Validation::checkReturnValueForAllExecutionPaths(ASTNode bodyNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *entity = ctx->getCurrentFunctionProcedureEntity();
        if (Utilities::getTypeOfEntity(entity) != MAliceEntityTypeFunction)
            return true;
        
        if (!checkHasReturnValueInAllExecutionPaths(bodyNode))
        {
            ctx->getErrorReporter()->reportError(ErrorFactory::createWarningError("Not all execution paths of function '" + entity->getIdentifier() + "' have a return value."));
            
            return false;
        }
        
        return true;
    }
    
    bool Validation::checkExpression(ASTNode node, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm)
    {
        return checkExpression(node, walker, false, ctx, typeConfirm);
    }
    
    bool Validation::checkExpression(ASTNode node, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm)
    {
        MAliceType type = MAliceTypeNone;
        if (!Utilities::getTypeFromExpressionNode(node, &type, requiresLValue, walker, ctx, NULL))
            return false;
        
        if (typeConfirm != type)
        {
            Range *exprRange = NULL;
            std::string exprText = Utilities::getNodeTextIncludingChildren(node, ctx, &exprRange);
            
            Error *error = NULL;
            
            if (type == MAliceTypeNone) {
                ASTNode firstNonImaginaryNode = Utilities::getFirstNonImaginaryChildNode(node);
                Error *error = ErrorFactory::createSemanticError("Can't match expected type '" +
                                                                 std::string(Utilities::getNameOfTypeFromMAliceType(typeConfirm)) +
                                                                 "' with procedure invocation.");
                error->setLineNumber(Utilities::getNodeLineNumber(firstNonImaginaryNode));
                error->setUnderlineRanges(Utilities::rangeToSingletonList(exprRange));
            }
            else
                error = ErrorFactory::createCannotMatchTypesError(node, typeConfirm, type, ctx);
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        return true;
    }
   
    bool Validation::checkCompatibleFunctionInvocationReturnType(ASTNode invocationNode, MAliceType expectedType, ASTWalker *walker, CompilerContext *ctx)
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
    
    bool Validation::checkSymbolForInvocationIsValidOrOutputError(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
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
            error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
            error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(identifierNode), Utilities::getNodeColumnIndex(identifierNode))));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        return true;
    }
    
    bool Validation::checkNumberOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = Utilities::getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        if (!funcProcEntity)
            return false;
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
        if (!identifierNode) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("number of invocation arguments"));
            return false;
        }
        
        unsigned int numberOfInvokedParameters = Utilities::getNumberOfChildNodes(identifierNode);
        unsigned int numberOfFormalParameters = (unsigned int)funcProcEntity->getParameterListTypes().size();
        
        if (numberOfFormalParameters != numberOfInvokedParameters) {
            Range *range = NULL;
            Utilities::getNodeTextIncludingChildren(invocationNode, ctx, &range);
            
            ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
            
            std::string funcProcHumanReadableType = funcProcEntity->humanReadableName();
            funcProcHumanReadableType[0] = toupper(funcProcHumanReadableType[0]);
            
            Error *error = ErrorFactory::createSemanticError(funcProcHumanReadableType + " '" +
                                                             funcProcEntity->getIdentifier() +
                                                             "' expects " +
                                                             Utilities::numberToString(numberOfFormalParameters) +
                                                             " " + ((numberOfFormalParameters == 1) ? "parameter" : "parameters") +
                                                             ", but found " +
                                                             Utilities::numberToString(numberOfInvokedParameters) +
                                                             " " + ((numberOfInvokedParameters == 1) ? "parameter" : "parameters") +
                                                             " in invocation.");
            error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(range));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        return true;
    }
    
    bool Validation::checkTypesOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = Utilities::getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        if (!funcProcEntity)
            return false;
        
        std::list<ParameterEntity> parameterTypes = funcProcEntity->getParameterListTypes();
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
        if (!identifierNode) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("function/procedure invocation"));
            return false;
        }
        
        unsigned int i = 0;
        for (std::list<ParameterEntity>::iterator it = parameterTypes.begin(); it != parameterTypes.end(); ++it) {
            ParameterEntity paramEntity = *it;
            ASTNode expressionNode = Utilities::getChildNodeAtIndex(identifierNode, i);
            if (!expressionNode)
                return false;
            
            MAliceType expressionType = MAliceTypeNone;
            bool passedByReference = false;
            if (!Utilities::getTypeFromExpressionNode(expressionNode, &expressionType, false, walker, ctx, &passedByReference))
                return false;

            // Check passedByReference is as expected
            
            if (passedByReference != paramEntity.isPassedByReference()) {
                Range *expressionRange = NULL;
                Utilities::getNodeTextIncludingChildren(expressionNode, ctx, &expressionRange);
                
                std::string funcProcIdentifier = Utilities::getFunctionProcedureInvocationIdentifier(invocationNode, walker, ctx);
                std::string expressionTypeString(Utilities::getNameOfTypeFromMAliceType(expressionType));
                std::string expectedTypeString(Utilities::getNameOfTypeFromMAliceType(paramEntity.getType()));
                
                Error *error = ErrorFactory::createSemanticError("Cannot match types of argument #" +
                                                                 Utilities::numberToString(i+1) +
                                                                 " in invocation of '" +
                                                                 funcProcIdentifier +
                                                                 "'. Expected argument passed by " +
                                                                 (passedByReference?"reference":"value") +
                                                                 " but found argument passed by " +
                                                                 (passedByReference?"value":"reference") +
                                                                 ".");
                error->setLineNumber(Utilities::getNodeLineNumber(expressionNode));
                error->setUnderlineRanges(Utilities::rangeToSingletonList(expressionRange));
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;

            }
            

                
            if (expressionType != paramEntity.getType()) {
                Range *expressionRange = NULL;
                Utilities::getNodeTextIncludingChildren(expressionNode, ctx, &expressionRange);
                
                std::string funcProcIdentifier = Utilities::getFunctionProcedureInvocationIdentifier(invocationNode, walker, ctx);
                std::string expressionTypeString(Utilities::getNameOfTypeFromMAliceType(expressionType));
                std::string expectedTypeString(Utilities::getNameOfTypeFromMAliceType(paramEntity.getType()));
                
                Error *error = ErrorFactory::createSemanticError("Cannot match types of argument #" +
                                                                 Utilities::numberToString(i+1) +
                                                                 " in invocation of '" +
                                                                 funcProcIdentifier +
                                                                 "'. Expected argument of type '" +
                                                                 expectedTypeString +
                                                                 "' but found '" +
                                                                 expressionTypeString +
                                                                 "'.");
                error->setLineNumber(Utilities::getNodeLineNumber(expressionNode));
                error->setUnderlineRanges(Utilities::rangeToSingletonList(expressionRange));
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
            
            ++i;
        }
        
        return true;
    }
    
    bool Validation::checkValidAssignmentStatementNode(ASTNode assignmentStatementNode, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(assignmentStatementNode) < 2) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("assignment statement"));
            return false;
        }
        
        ASTNode lvalueNode = Utilities::getChildNodeAtIndex(assignmentStatementNode, 0);
        ASTNode rvalueNode = Utilities::getChildNodeAtIndex(assignmentStatementNode, 1);
        ASTNode nodeBuffer;
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
            nodeBuffer = lvalueNode;
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
        
        Entity *symbolTableEntity = NULL;
        
        // Check lvalue exists on the symbol table
        if (!ctx->isSymbolInScope(lvalueIdentifier, &symbolTableEntity)) {
            Error *error = ErrorFactory::createSemanticError("Cannot find variable declaration for '" + lvalueIdentifier + "'.");
            error->setLineNumber(Utilities::getNodeLineNumber(lvalueNode));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(lvalueNode), Utilities::getNodeColumnIndex(lvalueNode))));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        } else {
            MAliceEntityType symbolTableEntityType = Utilities::getTypeOfEntity(symbolTableEntity);
            
            if (symbolTableEntityType == MAliceEntityTypeArray && !isLValueArray) {
                Error *error = ErrorFactory::createSemanticError("Trying to assign to array '" + lvalueIdentifier + "' directly is invalid.");
                error->setLineNumber(Utilities::getNodeLineNumber(lvalueNode));
                error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(lvalueNode), Utilities::getNodeColumnIndex(lvalueNode))));
                error->setAdditionalInformation("To fix, assign to one of its elements.");
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
            
            if (symbolTableEntityType == MAliceEntityTypeVariable && isLValueArray) {
                Range *errorRange = NULL;
                
                std::string lvalueText = Utilities::getNodeTextIncludingChildren(parentNode, ctx, &errorRange);
                
                Error *error = ErrorFactory::createSemanticError("Cannot assign value to '" + lvalueText + "' as '" + lvalueIdentifier + "' is not an array.");
                error->setLineNumber(Utilities::getNodeLineNumber(lvalueNode));
                error->setUnderlineRanges(Utilities::rangeToSingletonList(errorRange));
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }

            if (isLValueArray)
            {
                int numChildren = Utilities::getNumberOfChildNodes(nodeBuffer);
                if (numChildren < 2)
                {
                    ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("assignment statement"));
                    return false;
                }
                ASTNode exprNode = Utilities::getChildNodeAtIndex(nodeBuffer, 1);
                // Check expression subscript evaluates to a number
                checkExpression(exprNode, true, walker, ctx, MAliceTypeNumber);
            }
            
            VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(symbolTableEntity);
            
            // Iterate through expression and return the type, producing errors where relevant, returning the type as rvalue
            return checkExpression(rvalueNode, walker, ctx, variableEntity->getType());
        }
    }
    
    bool Validation::checkValidIfStatementNode(ASTNode ifStatementNode, ASTWalker *walker, CompilerContext *ctx)
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
    
    bool Validation::checkValidPrintStatementNode(ASTNode printStatementNode, ASTWalker *walker, CompilerContext *ctx)
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
            MAliceType type = MAliceTypeNone;
            
            if (!Utilities::getTypeFromExpressionNode(childNode, &type, true, walker, ctx, NULL))
                return false;
            
            if (type == MAliceTypeNone)
            {
                Error *error = ErrorFactory::createSemanticError("Expression: '" + Utilities::getNodeTextIncludingChildren(childNode, ctx, NULL) + "' is not a valid print statement.");
                error->setLineNumber(Utilities::getNodeLineNumber(printStatementNode));
                error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(printStatementNode), Utilities::getNodeColumnIndex(printStatementNode))));
                
                // Deepest child Node
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
        }
        
        return true;
    }

    bool Validation::checkHasReturnValueInAllExecutionPaths(ASTNode bodyNode)
    {
        ASTNode childNode;
        int numberOfIfStatements = 0;
        int numberOfReturningIfStatements = 0;
        bool ExpressionRequiredAlways = false;
        int numChildren = Utilities::getNumberOfChildNodes(bodyNode);
        for (int i = 0; i < numChildren; i++)
        {
            childNode = Utilities::getChildNodeAtIndex(bodyNode, i);
            switch(Utilities::getNodeType(childNode))
            {
                case IFSTATEMENT:
                    {
                    // If we find a return on an upper scope, there's no problem, it'll return true
                    // If every lower scope has a return statement, no problem
                    // If every lower scope requires an expression (all might potentially fail), it should be false
                    // Else, warning
                    for (int i = 0 ; i < Utilities::getNumberOfChildNodes(childNode); i++)
                    {
                        ASTNode ifChildNode = Utilities::getChildNodeAtIndex(childNode,i);
                        switch(Utilities::getNodeType(ifChildNode ))
                        {
                            case EXPRESSION:
                                numberOfIfStatements++;                                
                                numberOfReturningIfStatements = 
                                    checkHasReturnValueInAllExecutionPaths(childNode)
                                    ?    numberOfReturningIfStatements + 1 
                                    :    numberOfReturningIfStatements;
                                break;
                            default:
                                 ExpressionRequiredAlways = ExpressionRequiredAlways || i % 2 == 0;

                        }
                    }


                    
                    }
                    break;
                case RETURNSTATEMENT:
                    return true;
                case BODY:
                case STATEMENTLIST:
                    return checkHasReturnValueInAllExecutionPaths(childNode);
                    break;

                default:
                    continue;

            }
        }
        // Only fails if there is no return in this scope, and there is at least one lower scope which has no return
        return numberOfIfStatements == numberOfReturningIfStatements && ExpressionRequiredAlways;
    }
    
    bool Validation::checkValidReturnStatementNode(ASTNode returnStatementNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *currentFunctionProcedureEntity = ctx->getCurrentFunctionProcedureEntity();
        MAliceEntityType type = Utilities::getTypeOfEntity(currentFunctionProcedureEntity);
        
        if (!currentFunctionProcedureEntity) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("return statement"));
            return false;
        }
        

        if (type != MAliceEntityTypeFunction)
        {
                Range *errorRange = NULL;
                
                Utilities::getNodeTextIncludingChildren(returnStatementNode, ctx, &errorRange);
                
                Error *error = ErrorFactory::createSemanticError("Cannot return a value in procedure: \'" + currentFunctionProcedureEntity->getIdentifier() + "\'");
                error->setLineNumber(Utilities::getNodeLineNumber(returnStatementNode));
                error->setUnderlineRanges(Utilities::rangeToSingletonList(errorRange));
                error->setAdditionalInformation("Define \'" + currentFunctionProcedureEntity->getIdentifier() + "\' as a function instead");
                ctx->getErrorReporter()->reportError(error);
                return false;
        }
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(currentFunctionProcedureEntity);
        
        ASTNode expressionNode = Utilities::getChildNodeAtIndex(returnStatementNode, 0);
        if (!expressionNode) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("return statement"));
            return false;
        }

        MAliceType expressionType = MAliceTypeNone;
        if (!Utilities::getTypeFromExpressionNode(expressionNode, &expressionType, false, walker, ctx, NULL))
            return false;
        
        if (expressionType != functionEntity->getReturnType()) {
            Range *range = NULL;
            std::string expressionString = Utilities::getNodeTextIncludingChildren(expressionNode, ctx, &range);
            
            Error *error = ErrorFactory::createSemanticError("The return value '" +
                                                             Utilities::stripLeadingAndTrailingCharacters(expressionString, '\'') +
                                                             "' does not match the return value of '" +
                                                             functionEntity->getIdentifier() +
                                                             "' declared on line " +
                                                             Utilities::numberToString(functionEntity->getLineNumber()) +
                                                             ".");
            error->setLineNumber(Utilities::getNodeLineNumber(expressionNode));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(range));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        return true;
    }
    
    bool Validation::checkReturnTypeForInvocation(ASTNode invocationNode, MAliceType type, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = Utilities::getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        
        MAliceEntityType entityType = Utilities::getTypeOfEntity(funcProcEntity);
        
        // Check for a valid procedure invocation
        if (entityType == MAliceEntityTypeProcedure) {
            std::string expectedType = std::string(Utilities::getNameOfTypeFromMAliceType(type));
            Range *range = NULL;
            Utilities::getNodeTextIncludingChildren(invocationNode, ctx, &range);
            
            Error *error = ErrorFactory::createSemanticError("Expected type '" + expectedType + "'. Procedure '" + funcProcEntity->getIdentifier() + "' does have a return type.");
            error->setLineNumber(Utilities::getNodeLineNumber(invocationNode));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(range));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(funcProcEntity);
        
        if (type != functionEntity->getReturnType()) {
            std::string expectedType = std::string(Utilities::getNameOfTypeFromMAliceType(type));
            std::string actualType = std::string(Utilities::getNameOfTypeFromMAliceType(functionEntity->getReturnType()));
            Range *range = NULL;
            Utilities::getNodeTextIncludingChildren(invocationNode, ctx, &range);
            
            Error *error = ErrorFactory::createSemanticError("Can't match expected type '" + expectedType + "' with actual type '" + actualType + "'.");
            error->setLineNumber(Utilities::getNodeLineNumber(invocationNode));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(range));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        return true;
    }
}
