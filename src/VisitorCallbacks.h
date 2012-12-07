#ifndef _VISITORCALLBACKS
#define _VISITORCALLBACKS

#include "CompilerContext.h"
#include "ASTWalker.h"
#include "ParameterEntity.h"

#include "llvm/Value.h"

namespace MAlice {
    
    class FunctionProcedureEntity;
    
    // Statements
    bool visitNullStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitAssignmentStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitIncrementStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitDecrementStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitIfStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitInputStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitPrintStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitReturnStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitStatementListNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitWhileStatementNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    
    // Expressions
    bool visitLogicalAndExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitLogicalOrExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitLogicalNotExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseAndExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseOrExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseXorExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitDivideExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitEqualsExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitGreaterThanExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitIdentifierNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitLessThanExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitLessThanOrEqualExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitMinusExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitModuloExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitMultiplyExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitNotEqualExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitPlusExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitTildeExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    
    // Imaginary nodes – used to improve AST structure
    bool visitArrayDeclarationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitArraySubscriptNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitBodyNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitByReferenceParameterNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitByValueParameterNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitDeclarationsNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitArithmeticExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitExpressionNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitFunctionDeclarationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitProcFuncInvocationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitParamsNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitProgramNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitProcedureDeclarationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitVariableDeclarationNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitArbitraryBlockNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    
    // Literals
    bool visitCharacterLiteralNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitNumberLiteralNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitStringLiteralNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    
    // Types
    bool visitLetterTypeNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitNumberTypeNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);
    bool visitSentenceTypeNode(ASTNode node, ValueList *outValueList, ASTWalker *walker, CompilerContext *ctx);

}

#endif