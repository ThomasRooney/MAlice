#ifndef _VISITORCALLBACKS
#define _VISITORCALLBACKS

#include "CompilerContext.h"
#include "ASTWalker.h"

namespace MAlice {


void visitCompoundNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitProgramNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);

void visitTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitConstantNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitFunctionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitProcedureNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitDeclarationArgumentListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitBlockNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitBodyNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitDeclarationArgumentNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitAssignmentNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitStatementInnerSeperatorNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitStatementListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitReturnNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitWhileLoopNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitIfBlockNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitElseBlockNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitPrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitSTDOUTLvalueNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitIncrementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitDecrementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitLvalueNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitAdditiveExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitMultiplicativeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitBitwiseExprNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitUnaryExprNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitBooleanExprNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitSingleBooleanExprNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
void visitNullStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);


}
#endif