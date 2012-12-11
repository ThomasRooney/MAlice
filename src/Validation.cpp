#include "Validation.h"

#include "ASTWalker.h"
#include "CompilerContext.h"
#include "ErrorReporter.h"
#include "ErrorFactory.h"
#include "FunctionEntity.h"
#include "Utilities.h"
#include "ProcedureEntity.h"
#include "SemanticChecks.h"
#include "VariableEntity.h"

namespace MAlice {

    bool Validation::validateAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidAssignmentStatementNode(node, walker, ctx);
    }

    bool Validation::validateArbitraryBlockNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        bool result;
        ctx->enterScope();
        result = walker->validateChildren(node, ctx);
        ctx->exitScope();
        return result;
    }

    bool Validation::validateBodyNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
        
        bool result = walker->validateChildren(node, ctx);
        
        ctx->exitScope();
        
        return result;
    }
    
    bool Validation::validateIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("increment statement"));
            return false;
        }
        
        // Check that the child of this node is a number, and it has a child
        return checkExpression(Utilities::getChildNodeAtIndex(node, 0), true, walker, ctx, Type(PrimitiveTypeNumber));
    }
    
    bool Validation::validateDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("decrement statement"));
            return false;
        }
        
        // Check that the child of this node is a number, and it has a child
        return checkExpression(Utilities::getChildNodeAtIndex(node,0), true, walker, ctx, Type(PrimitiveTypeNumber));
    }

    bool Validation::validateIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        int numChildren = Utilities::getNumberOfChildNodes(node);
        if (numChildren < 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("if statement"));
            return false;
        }

        for (int i = 0 ; i < numChildren; i++)
        {
            ASTNode childNode = Utilities::getChildNodeAtIndex(node,i);
            ANTLR3_UINT32 nodeType = Utilities::getNodeType(childNode);
            
            if (nodeType == EXPRESSION) {
                if (!checkExpression(childNode, walker, ctx, Type(PrimitiveTypeBoolean)))
                    return false;
            } else if (i == 0) {
                ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("if statement"));
                
                // First node needs to be an expression
                return false;
            }
        }
        
        return true;
    }

    bool Validation::validateInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        // input must be either a number or a letter.
        if (Utilities::getNumberOfChildNodes(node) < 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("input Statement"));
            return false;
        }
        Type t;
        
        if (!Utilities::getTypeFromExpressionNode(node, &t, true, walker, ctx, NULL))
            return false;

        if (t.isArray()) {
            Range *r = NULL;
            
            ASTNode invalidLValueNode = Utilities::getFirstNonImaginaryChildNode(node);
            std::string text = Utilities::getNodeTextIncludingChildren(invalidLValueNode, ctx, &r);
            Error *error = ErrorFactory::createSemanticError("Cannot input directly to an array.");
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(r));
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidLValueError(invalidLValueNode, ctx));
            
            return false;
        }
        
        PrimitiveType primitiveType = t.getPrimitiveType();
        
        if (primitiveType != PrimitiveTypeLetter && primitiveType != PrimitiveTypeNumber) {
            Range *r = NULL;
            std::string text = Utilities::getNodeTextIncludingChildren(node, ctx, &r);
            Error *error = ErrorFactory::createSemanticError("Input can only stream to a Letter or a Number variable.  '" + text + "' is neither.");
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(r));
        }

        return true;
    }
    
    bool Validation::validatePrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidPrintStatementNode(node, walker, ctx);
    }
    
    bool Validation::validateReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return checkValidReturnStatementNode(node, walker, ctx);
    }

    bool Validation::validateWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        // First child must be an expression
        int numChildren = Utilities::getNumberOfChildNodes(node);
        if (numChildren < 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("while Statement with no conditional"));
            return false;
        }
        //  the expression on node 1 must return a a boolean
        ASTNode exprNode = Utilities::getChildNodeAtIndex(node,0);
        ANTLR3_UINT32 nodeType = Utilities::getNodeType(exprNode);
        
        if (nodeType != EXPRESSION)
            return false;
        
        if (!checkExpression(exprNode, walker, ctx, Type(PrimitiveTypeBoolean)))
            return false;
        
        ASTNode bodyNode = Utilities::getChildNodeAtIndex(node, 1);
        if (!bodyNode) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("while loop"));
            return false;
        }
        
        return walker->validateNode(bodyNode, ctx);
    }
    
    bool Validation::validateArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode == NULL)
            return false;
        
        std::string identifier(Utilities::getNodeText(identifierNode));
        std::string type;
        if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
            return false;
        
        if (Utilities::getNumberOfChildNodes(identifierNode) != 2) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("array declaration"));
            return false;
        }
    
        ASTNode typeNode = Utilities::getChildNodeAtIndex(identifierNode, 0);
        if (typeNode == NULL) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("array declaration"));
            return false;
        }
        
        type = Utilities::getNodeText(typeNode);
        
        ASTNode exprNode = Utilities::getChildNodeAtIndex(identifierNode, 1);
        if (!checkExpression(exprNode, false, walker, ctx, Type(PrimitiveTypeNumber)))
            return false;
        
        std::string typeString = Utilities::getNodeText(typeNode);
        Type arrayType = Utilities::getTypeFromTypeString(typeString);
        arrayType.setIsArray(true);
        
        VariableEntity *variableEntity = new VariableEntity(identifier, Utilities::getNodeLineNumber(node), arrayType);
        ctx->addEntityInScope(identifier, variableEntity);
        
        return true;
    }
    
    bool Validation::validateFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode == NULL) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("function declaration"));
            return false;
        }
        
        std::string identifier(Utilities::getNodeText(identifierNode));
        
        if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
            return false;
        
        bool hasParams = false;
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            hasParams = true;
        ASTNode returnNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
        if (returnNode == NULL) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("function declaration"));
            return false;
        }

        // get node index 1, if its a parameter node, get params...
        std::string returnString = Utilities::getNodeText(returnNode);
        
        Type returnType = Utilities::getTypeFromTypeString(returnString);
        
        FunctionEntity *functionEntity = new FunctionEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::vector<ParameterEntity*>(), returnType);
        ctx->addEntityInScope(identifier, functionEntity);
        ctx->pushFunctionProcedureEntity(functionEntity);
        
        // Populate the function arguments.
        if (hasParams) {
            if (!walker->validateNode(nodeI1, ctx))
                return false;
        }
        
        // Walk through the children
        // Get Body node, check that it has all return paths correctly
        
        ASTNode bodyNode = Utilities::getChildNodeAtIndex(node, hasParams?3:2);
        Validation::checkReturnValueForAllExecutionPaths(bodyNode, walker, ctx); // Generates warning

        bool result = walker->validateChildren(node, ctx);
        
        ctx->popFunctionProcedureEntity();
        
        return result;
    }
    
    bool Validation::validateParamsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *entity = ctx->getCurrentFunctionProcedureEntity();
        std::vector<ParameterEntity*> parameterList = Utilities::getParameterTypesFromParamsNode(node);
        
        for (auto it = parameterList.begin(); it != parameterList.end(); ++it) {
            ParameterEntity *entity = *it;
            ctx->addEntityInScope(entity->getIdentifier(), entity);
        }
        
        entity->setParameterListTypes(parameterList);
        
        return true;
    }

    bool Validation::validateExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        bool result;
        ctx->beginExpression();
        result = walker->validateChildren(node, ctx);
        ctx->endExpression();
        return result;

    }
    
    bool Validation::validateProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!checkSymbolForInvocationIsValidOrOutputError(node, walker, ctx))
            return false;
        
        if (!Utilities::getReturnTypeForInvocation(node, walker, ctx).isVoid() && !ctx->withinExpression()) {
            FunctionProcedureEntity *funcProcEntity = Utilities::getFunctionProcedureEntityForInvocationNode(node, walker, ctx);
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
    
    bool Validation::validateProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        if (identifierNode == NULL) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("procedure declaration"));
            return false;
        }
        
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
            return false;
        
        ProcedureEntity *procedureEntity = new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::vector<ParameterEntity*>());
        
        ctx->addEntityInScope(identifier, procedureEntity);
        ctx->pushFunctionProcedureEntity(procedureEntity);
        
        bool hasParams = false;
        
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            hasParams = true;
        ASTNode bodyNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
        
        if (hasParams) {
            if (!walker->validateNode(nodeI1, ctx))
                return false;
        }

        walker->validateNode(bodyNode, ctx);
        
        ctx->popFunctionProcedureEntity();
        
        return true;
    }
    
    bool Validation::validateVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (ctx->isKeyword(identifier)) {
                Error *error = ErrorFactory::createSemanticError("Cannot declare variable '" + identifier + "' because it is a keyword.");
                error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
                
                error->setArrowRanges(Utilities::rangeToSingletonList(
                                                                      Range::create(Utilities::getNodeLineNumber(identifierNode), Utilities::getNodeColumnIndex(identifierNode))
                                                                      ));
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
            
            checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx);
            
            Type type = Utilities::getTypeFromTypeString(std::string(Utilities::getNodeText(typeNode)));
            
            // See if a third node exists - if so this is a variable assignment node that we want to visit
            
            ASTNode thirdNode = Utilities::getChildNodeAtIndex(node, 2);
            if (thirdNode != NULL)
                checkExpression(thirdNode, walker, ctx, type);
            
        }
        
        std::string identifier = Utilities::getNodeText(identifierNode);
        std::string typeString = Utilities::getNodeText(typeNode);
        
        VariableEntity *variable = new VariableEntity(identifier,
                                                      Utilities::getNodeLineNumber(identifierNode),
                                                      Utilities::getTypeFromTypeString(typeString));
        
        ctx->addEntityInScope(identifier, variable);

        return true;
    }

}