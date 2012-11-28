
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
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node))));
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
        unsigned int numChildNodes = Utilities::getNumberOfChildNodes(node);
        ASTNode bodyNode;
        bool hasBodyFlag = false;
        ctx->enterScope();
        
        // Loop through the rest of the child nodes
        for (unsigned int i = 1; i < numChildNodes; ++i) {
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
                    bodyNode = childNode;
                    walker->visitNode(bodyNode, ctx);
                    hasBodyFlag = true;
                    break;
                default:
                    break;
            }
        }
        // Warning if not all execution paths return the type of the entity, should this be a function
        if (hasBodyFlag && Utilities::getTypeOfEntity(entity) == MAliceEntityTypeFunction)
        {   
            if (!checkHasReturnValueInAllExecutionPaths(bodyNode))
            {
                ctx->getErrorReporter()->reportError(ErrorFactory::createWarningError("Not all execution paths of function '" + entity->getIdentifier() + "' have a return value."));
            }
        }
        ctx->exitScope();
        
        return true;
    }
    
    bool checkExpression(ASTNode node, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm)
    {
        return checkExpression(node, walker, false, ctx, typeConfirm);
    }
    
    bool checkExpression(ASTNode node, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm)
    {
        MAliceType type = MAliceTypeNone;
        if (!getTypeFromExpressionNode(node, &type, requiresLValue, walker, ctx, NULL))
            return false;
        
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
                Utilities::stripLeadingAndTrailingCharacters(exprText, '\'') + "'.";
            }
            
            ASTNode firstNonImaginaryNode = Utilities::getFirstNonImaginaryChildNode(node);
            Error *error = ErrorFactory::createSemanticError(errorMessage);
            error->setLineNumber(Utilities::getNodeLineNumber(firstNonImaginaryNode));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(exprRange));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        return true;
    }
    
    bool getTypeFromExpressionNode(ASTNode node, MAliceType *outType, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, bool *passedByReference)
    {
        ASTNode firstChildNode = Utilities::getChildNodeAtIndex(node, 0);
        if (!firstChildNode) {
            // Possibly the firstChildNode has been passed in directly to this function. For example in the case of 'ate'/'drank'
            firstChildNode = node;
        }
        
        // Check identifiers first, because on their own they can reference an array if we don't want an l-value
        if (!requiresLValue && Utilities::getNodeType(firstChildNode) == IDENTIFIER) {
            MAliceType type = MAliceTypeNone;
            MAliceEntityType entityType = MAliceEntityTypeUndefined;

            if (!getTypeFromExpressionIdentifierNode(firstChildNode, &type, &entityType, walker, ctx))
                return false;

            if (passedByReference)
                *passedByReference = entityType==MAliceEntityTypeArray;
            
            if (outType)
                *outType = type;
            
            return true;
        }
        
        return getTypeFromExpressionRuleNode(firstChildNode, outType, walker, ctx);
    }
    
    bool getTypeFromExpressionRuleNode(ASTNode node, MAliceType *outType, ASTWalker *walker, CompilerContext *ctx)
    {
        switch (Utilities::getNodeType(node))
        {
            case IDENTIFIER:
            {
                MAliceType type = MAliceTypeNone;
                MAliceEntityType entityType = MAliceEntityTypeUndefined;
                
                if (!getTypeFromExpressionIdentifierNode(node, &type, &entityType, walker, ctx))
                    return false;
                
                if (entityType == MAliceEntityTypeArray) {
                    // Array has no children, hence this is an error!
                    Error *error = ErrorFactory::createSemanticError("Cannot refer to array '" + std::string(Utilities::getNodeText(node)) + "' directly.");
                    error->setLineNumber(Utilities::getNodeLineNumber(node));
                    error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node))));
                    error->setAdditionalInformation("To fix, specify an array element.");
                    ctx->getErrorReporter()->reportError(error);
                }
                
                if (outType)
                    *outType = type;
                
                return true;
            }
                break;
            case INVOCATION:
                return getTypeFromInvocationExpressionNode(node, outType, walker, ctx);
                break;
            case EQUALS:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "equality", MAliceTypeNumber | MAliceTypeLetter | MAliceTypeSentence, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case NOTEQUAL:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "inequality", MAliceTypeNumber | MAliceTypeLetter | MAliceTypeSentence, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case LESSTHAN:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "less than", MAliceTypeNumber | MAliceTypeLetter, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case LESSTHANEQUAL:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "less than or equal", MAliceTypeNumber | MAliceTypeLetter, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case GREATERTHAN:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "greater than", MAliceTypeNumber | MAliceTypeLetter, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case GREATERTHANEQUAL:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "greater than or equal", MAliceTypeNumber | MAliceTypeLetter, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case ARRAYSUBSCRIPT:
            {
                ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
                if (!identifierNode || Utilities::getNodeType(identifierNode) != IDENTIFIER) {
                    ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("array subscript"));
                    return false;
                }
                
                std::string identifier = Utilities::getNodeText(identifierNode);
                
                MAliceEntityType entityType = MAliceEntityTypeUndefined;
                if (!getTypeFromExpressionIdentifierNode(identifierNode, NULL, &entityType, walker, ctx))
                    return false;
                
                if (entityType != MAliceEntityTypeArray) {
                    Range *identifierRange = NULL;
                    Utilities::getNodeTextIncludingChildren(node, ctx, &identifierRange);
                    
                    Error *error = ErrorFactory::createSemanticError("Invalid array subscript: '" + identifier + "' does not reference an array.");
                    error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
                    error->setUnderlineRanges(Utilities::rangeToSingletonList(identifierRange));
                    
                    ctx->getErrorReporter()->reportError(error);
                    
                    return false;
                }
                
                Entity *entity = NULL;
                ctx->isSymbolInScope(identifier, &entity);
                
                ArrayEntity *arrayEntity = dynamic_cast<ArrayEntity*>(entity);
                if (!arrayEntity) {
                    ctx->getErrorReporter()->reportError(ErrorFactory::createInternalError("Could not get valid entity from symbol table for array '" + identifier + "'."));
                    return false;
                }
                
                if (outType)
                    *outType = arrayEntity->getType();

                // Check the expression subscript has number type
                ASTNode exprNode = Utilities::getChildNodeAtIndex(node, 1);
                checkExpression(exprNode,false,walker,ctx,MAliceTypeNumber);
                
                return true;
            }
                break;
            case PLUS:
            {
                if (Utilities::getNumberOfChildNodes(node) == 2) {
                    MAliceType type = MAliceTypeNone;
                    if (!getTypeFromBinaryOperatorNode(node, &type, "addition", MAliceTypeNumber | MAliceTypeLetter | MAliceTypeSentence, walker, ctx))
                        return false;
                    
                    if (outType)
                        *outType = type;
                } else {
                    if (!getTypeFromUnaryOperatorNode(node, NULL, "unary plus", MAliceTypeNumber, walker, ctx))
                        return false;
                    
                    if (outType)
                        *outType = MAliceTypeNumber;
                }
            }
                break;
            case MINUS:
            {
                if (Utilities::getNumberOfChildNodes(node) == 2) {
                    MAliceType type = MAliceTypeNone;
                    if (!getTypeFromBinaryOperatorNode(node, &type, "subtraction", MAliceTypeNumber | MAliceTypeLetter | MAliceTypeSentence, walker, ctx))
                        return false;
                    
                    if (outType)
                        *outType = type;
                } else {
                    if (!getTypeFromUnaryOperatorNode(node, NULL, "unary minus", MAliceTypeNumber, walker, ctx))
                        return false;
                    
                    if (outType)
                        *outType = MAliceTypeNumber;
                }
            }
                break;
            case TILDE:
            {
                if (!getTypeFromUnaryOperatorNode(node, NULL, "tilde", MAliceTypeNumber, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeNumber;
            }
                break;
            case BANG:
            {
                if (!getTypeFromUnaryOperatorNode(node, NULL, "unary NOT", MAliceTypeBoolean, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case BITWISEAND:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "bitwise AND", MAliceTypeNumber, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeNumber;
            }
                break;
            case BITWISEOR:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "bitwise OR", MAliceTypeNumber, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeNumber;
            }
                break;
            case BITWISEXOR:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "bitwise XOR", MAliceTypeNumber, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeNumber;
            }
                break;
            case MODULO:
            {
                MAliceType type = MAliceTypeNone;
                if (!getTypeFromBinaryOperatorNode(node, &type, "modulo", MAliceTypeNumber, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = type;
            }
                break;
            case MULTIPLY:
            {
                MAliceType type = MAliceTypeNone;
                if (!getTypeFromBinaryOperatorNode(node, &type, "multiplication", MAliceTypeNumber, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = type;
            }
                break;
            case DIVIDE:
            {
                MAliceType type = MAliceTypeNone;
                if (!getTypeFromBinaryOperatorNode(node, &type, "division", MAliceTypeNumber, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = type;
            }
                break;
            case LOGICALAND:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "logical and", MAliceTypeBoolean, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case LOGICALOR:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "logical or", MAliceTypeBoolean, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = MAliceTypeBoolean;
            }
                break;
            case NUMBER_LITERAL:
                if (outType)
                    *outType = MAliceTypeNumber;
                break;
            case CHARACTER_LITERAL:
                if (outType)
                    *outType = MAliceTypeLetter;
                break;
            case STRING_LITERAL:
                if (outType)
                    *outType = MAliceTypeSentence;
                break;
            default:
                break;
        }
        
        return true;
    }
    
    bool getTypeFromInvocationExpressionNode(ASTNode node, MAliceType *outType, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!checkSymbolForInvocationIsValidOrOutputError(node, walker, ctx))
            return false;
        
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(node, walker, ctx);
        MAliceEntityType funcProcType = Utilities::getTypeOfEntity(funcProcEntity);
        if (funcProcType != MAliceEntityTypeFunction && funcProcType != MAliceEntityTypeProcedure) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("identifier in expression"));
            return false;
        }
        
        if (funcProcType == MAliceEntityTypeProcedure) {
            Range *procCallRange = NULL;
            Utilities::getNodeTextIncludingChildren(node, ctx, &procCallRange);
            
            Error *error = ErrorFactory::createSemanticError("Cannot use void result of procedure '" +
                                                             funcProcEntity->getIdentifier() +
                                                             "' in expression.");
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(procCallRange));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        MAliceType returnType = getReturnTypeForInvocation(node, walker, ctx);
        if (outType)
            *outType = returnType;
        
        return true;
    }
    
    bool getTypeFromBinaryOperatorNode(ASTNode node, MAliceType *outType, std::string operatorName, unsigned int requiredTypes, ASTWalker *walker, CompilerContext *ctx)
    {
        MAliceType t1, t2;
        unsigned int numChildNodes = Utilities::getNumberOfChildNodes(node);
        
        if (numChildNodes != 2) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError(operatorName +
                                                                                     " operator in expression " +
                                                                                     Utilities::getOperatorStringFromOperatorNode(node) +
                                                                                     "."));
            return false;
        }
        
        ASTNode leftOperandRootNode = Utilities::getChildNodeAtIndex(node, 0);
        ASTNode rightOperandRootNode = Utilities::getChildNodeAtIndex(node, 1);
        
        if (!getTypeFromExpressionRuleNode(leftOperandRootNode, &t1, walker, ctx))
            return false;
        
        if (!getTypeFromExpressionRuleNode(rightOperandRootNode, &t2, walker, ctx))
            return false;
        
        ;
        
        if (t1 != t2) {
            Error *error = ErrorFactory::createSemanticError("Cannot match types '" +
                                                             std::string(Utilities::getNameOfTypeFromMAliceType(t1)) +
                                                             "' and '" +
                                                             std::string(Utilities::getNameOfTypeFromMAliceType(t2)) +
                                                             "' for operands to binary operator.");
            
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            
            Range *leftOperandRange = NULL, *rightOperandRange = NULL;
            Utilities::getNodeTextIncludingChildren(leftOperandRootNode, ctx, &leftOperandRange);
            Utilities::getNodeTextIncludingChildren(rightOperandRootNode, ctx, &rightOperandRange);
            
            std::list<Range*> underlineRanges;
            underlineRanges.push_back(leftOperandRange);
            underlineRanges.push_back(rightOperandRange);
            
            error->setUnderlineRanges(underlineRanges);
            
            unsigned int operatorStringLength = (unsigned int)Utilities::getOperatorStringFromOperatorNode(node).size();
            Range *arrowRange = Utilities::createRange(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node),
                                                       Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node) + operatorStringLength - 1);
            
            error->setArrowRanges(Utilities::rangeToSingletonList(arrowRange));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        if ((t1 & requiredTypes) == 0) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidOperandTypeError(node, leftOperandRootNode, t1, requiredTypes, ctx));
            
            return false;
        }
        
        if ((t2 & requiredTypes) == 0) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidOperandTypeError(node, rightOperandRootNode, t2, requiredTypes, ctx));
            
            return false;
        }
        
        if (outType)
            *outType = t1;
    
        return true;
    }
    
    bool getTypeFromUnaryOperatorNode(ASTNode node, MAliceType *outType, std::string operatorName, MAliceType requiredType, ASTWalker *walker, CompilerContext *ctx)
    {
        MAliceType type;
        unsigned int numChildNodes = Utilities::getNumberOfChildNodes(node);
        
        if (numChildNodes != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError(operatorName + " operator in expression"));
            return false;
        }
        
        ASTNode operandRootNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (!getTypeFromExpressionRuleNode(operandRootNode, &type, walker, ctx))
            return false;
        
        if (requiredType != MAliceTypeNone) {
            if (type != requiredType) {
                ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidOperandTypeError(operandRootNode, requiredType, type, ctx));
                
                return false;
            }
        }
        
        if (outType)
            *outType = type;
        
        return true;
    }
    
    bool getTypeFromExpressionIdentifierNode(ASTNode node, MAliceType *outType, MAliceEntityType *outEntityType, ASTWalker *walker, CompilerContext *ctx)
    {
        Entity *lookupEntity;
        
        std::string identifier = Utilities::getNodeText(node);

        if (!ctx->isSymbolInScope(identifier, &lookupEntity)) {
            Range *range = NULL;
            
            Utilities::getNodeTextIncludingChildren(node, ctx, &range);
            
            Error *error = ErrorFactory::createSemanticError("Identifier: '" + identifier + "' is not in scope.");
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(range));
            
            ctx->getErrorReporter()->reportError(error);
            
            return MAliceTypeNone;
        }
        
        VariableEntity *lookupVEntity = dynamic_cast<VariableEntity*>(lookupEntity);
        if (!lookupVEntity) {
            Error *error = ErrorFactory::createSemanticError("Identifier '" + identifier + "' does not have a type.");
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node))));
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        if (outType)
            *outType = lookupVEntity->getType();
        
        if (outEntityType)
            *outEntityType = Utilities::getTypeOfEntity(lookupVEntity);
        
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
            error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
            error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(identifierNode), Utilities::getNodeColumnIndex(identifierNode))));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        return true;
    }
    
    MAliceType getReturnTypeForInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        if (!funcProcEntity) {
            return MAliceTypeNone;
        }
        
        MAliceEntityType entityType = Utilities::getTypeOfEntity(funcProcEntity);
        
        if (entityType == MAliceEntityTypeProcedure)
            return MAliceTypeNone;
        
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(funcProcEntity);
        
        return functionEntity->getReturnType();
    }
    
    bool checkReturnTypeForInvocation(ASTNode invocationNode, MAliceType type, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        
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
    
    bool checkNumberOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
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
    
    bool checkTypesOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
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
            if (!getTypeFromExpressionNode(expressionNode, &expressionType, false, walker, ctx, &passedByReference))
                return false;

            // Check passedByReference is as expected
            
            if (passedByReference != paramEntity.isPassedByReference()) {
                Range *expressionRange = NULL;
                Utilities::getNodeTextIncludingChildren(expressionNode, ctx, &expressionRange);
                
                std::string funcProcIdentifier = getFunctionProcedureInvocationIdentifier(invocationNode, walker, ctx);
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
                
                std::string funcProcIdentifier = getFunctionProcedureInvocationIdentifier(invocationNode, walker, ctx);
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
    
    bool checkValidAssignmentStatementNode(ASTNode assignmentStatementNode, ASTWalker *walker, CompilerContext *ctx)
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
        
        // TODO: Check these are not null. otherwise fatal error
        
        
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
            MAliceType type = MAliceTypeNone;
            
            if (!getTypeFromExpressionNode(childNode, &type, true, walker, ctx, NULL))
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

    bool checkHasReturnValueInAllExecutionPaths(ASTNode bodyNode)
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
    
    bool checkValidReturnStatementNode(ASTNode returnStatementNode, ASTWalker *walker, CompilerContext *ctx)
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

        }
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(currentFunctionProcedureEntity);
        
        ASTNode expressionNode = Utilities::getChildNodeAtIndex(returnStatementNode, 0);
        if (!expressionNode) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("return statement"));
            return false;
        }

        MAliceType expressionType = MAliceTypeNone;
        if (!getTypeFromExpressionNode(expressionNode, &expressionType, false, walker, ctx, NULL))
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
}
