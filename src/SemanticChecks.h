
#ifndef _MALICESEMANTICCHECKS
#define _MALICESEMANTICCHECKS

#include <iostream>
#include <list>

#include "Types.h"

namespace MAlice {
    
    class FunctionProcedureEntity;
    class ParameterEntity;
  
    bool checkSymbolNotInCurrentScopeOrOutputError(std::string identifier, ASTNode node, CompilerContext *ctx);
    bool checkCompatibleFunctionInvocationReturnType(ASTNode invocationNode, MAliceType expectedType, ASTWalker *walker, CompilerContext *ctx);
    
    FunctionProcedureEntity *getFunctionProcedureEntityForInvocationNode(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    std::string getFunctionProcedureInvocationIdentifier(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    MAliceType getReturnTypeForInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkSymbolForInvocationIsValidOrOutputError(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkReturnTypeForInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkNumberOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkTypesOfArgumentsForInvocationIsValid(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkHasReturnValueInAllExecutionPaths(ASTNode bodyNode);
    std::list<ParameterEntity> getParameterTypesFromParamsNode(ASTNode paramsNode);
    bool checkExpression(ASTNode node, ASTWalker *walker, CompilerContext *ctx, MAliceType type);
    bool checkExpression(ASTNode node, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, MAliceType type);
    
    bool getTypeFromExpressionNode(ASTNode node, MAliceType *outType, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, bool *passedByReference);
    bool getTypeFromExpressionRuleNode(ASTNode node, MAliceType *outType, ASTWalker *walker, CompilerContext *ctx);
    bool getTypeFromInvocationExpressionNode(ASTNode node, MAliceType *outType, ASTWalker *walker, CompilerContext *ctx);
    bool getTypeFromBinaryOperatorNode(ASTNode node, MAliceType *outType, std::string operatorName, unsigned int requiredTypes, ASTWalker *walker, CompilerContext *ctx);
    bool getTypeFromUnaryOperatorNode(ASTNode node, MAliceType *outType, std::string operatorName, MAliceType requiredType, ASTWalker *walker, CompilerContext *ctx);
    bool getTypeFromExpressionIdentifierNode(ASTNode node, MAliceType *outType, MAliceEntityType *outEntityType, ASTWalker *walker, CompilerContext *ctx);
    
    bool checkValidAssignmentStatementNode(ASTNode assignmentStatementNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkValidIfStatementNode(ASTNode ifStatementNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkValidPrintStatementNode(ASTNode printStatementNode, ASTWalker *walker, CompilerContext *ctx);
    bool checkValidReturnStatementNode(ASTNode returnStatementNode, ASTWalker *walker, CompilerContext *ctx);
    
    bool checkReturnValueForAllExecutionPaths(ASTNode bodyNode, ASTWalker *walker, CompilerContext *ctx);
};

#endif /* #ifndef _MALICESEMANTICCHECKS */
