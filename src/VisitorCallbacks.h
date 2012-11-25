#ifndef _VISITORCALLBACKS
#define _VISITORCALLBACKS

#include "CompilerContext.h"
#include "ASTWalker.h"
#include "ParameterEntity.h"

namespace MAlice {
    
    class FunctionProcedureEntity;
    
    // Statements
    bool visitNullStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitPrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitStatementListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    
    // Expressions
    bool visitLogicalAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitLogicalOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitLogicalNotExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseXorExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitDivideExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitEqualsExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitGreaterThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitIdentifierNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitLessThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitLessThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitMinusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitModuloExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitMultiplyExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitNotEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitPlusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitTildeExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    
    // Imaginary nodes – used to improve AST structure
    bool visitArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitArraySubscriptNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitBodyNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitByReferenceParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitByValueParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitDeclarationsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitArithmeticExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitBooleanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitParamsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    
    // Literals
    bool visitCharacterLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitNumberLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitStringLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    
    // Types
    bool visitLetterTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitNumberTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    bool visitSentenceTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
    
    // Helper methods
    bool checkSymbolNotInCurrentScopeOrOutputError(std::string identifier, ASTNode node, CompilerContext *ctx);
    bool visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(ASTNode node, FunctionProcedureEntity *entity, ASTWalker *walker, CompilerContext *ctx);
    std::list<ParameterEntity> getParameterTypesFromParamsNode(ASTNode paramsNode);
}

#endif