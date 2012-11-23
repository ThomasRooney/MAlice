#ifndef _MALICEASTWALKER
#define _MALICEASTWALKER

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include <vector>
#include "CompilerContext.h"

#define NUMBER_OF_VISIT_FUNCTIONS 34

namespace MAlice {

typedef pANTLR3_BASE_TREE ASTNode;

class ASTWalker {
public:
  ASTWalker();
  void feedTree();
  bool validateTree(ASTNode node, CompilerContext *ctx);
  void compileTree();
  pANTLR3_STRING printTree(ASTNode node, int);
  void initialiseSymbolTable();
  ~ASTWalker();
protected:
  SymbolTable *rootSymbolTable;

private:
  void visitNode(ASTNode node, CompilerContext *ctx);
  void visitCompoundNode(ASTNode node, CompilerContext *ctx);
  void visitProgramNode(ASTNode node, CompilerContext *ctx);
  void visitTypeNode(ASTNode node, CompilerContext *ctx);
  void visitConstantNode(ASTNode node, CompilerContext *ctx);
  void visitFunctionNode(ASTNode node, CompilerContext *ctx);
  void visitProcedureNode(ASTNode node, CompilerContext *ctx);
  void visitDeclarationArgumentListNode(ASTNode node, CompilerContext *ctx);
  void visitBlockNode(ASTNode node, CompilerContext *ctx);
  void visitBodyNode(ASTNode node, CompilerContext *ctx);
  void visitDeclarationArgumentNode(ASTNode node, CompilerContext *ctx);
  void visitFuncInvocationNode(ASTNode node, CompilerContext *ctx);
  void visitAssignmentNode(ASTNode node, CompilerContext *ctx);
  void visitStatementInnerSeperatorNode(ASTNode node, CompilerContext *ctx);
  void visitStatementListNode(ASTNode node, CompilerContext *ctx);
  void visitStatementNode(ASTNode node, CompilerContext *ctx);
  void visitReturnNode(ASTNode node, CompilerContext *ctx);
  void visitWhileLoopNode(ASTNode node, CompilerContext *ctx);
  void visitIfBlockNode(ASTNode node, CompilerContext *ctx);
  void visitElseBlockNode(ASTNode node, CompilerContext *ctx);
  void visitVariableDeclarationNode(ASTNode node, CompilerContext *ctx);
  void visitPrintStatementNode(ASTNode node, CompilerContext *ctx);
  void visitInputStatementNode(ASTNode node, CompilerContext *ctx);
  void visitSTDOUTLvalueNode(ASTNode node, CompilerContext *ctx);
  void visitIncrementNode(ASTNode node, CompilerContext *ctx);
  void visitDecrementNode(ASTNode node, CompilerContext *ctx);
  void visitExpressionNode(ASTNode node, CompilerContext *ctx);
  void visitLvalueNode(ASTNode node, CompilerContext *ctx);
  void visitAdditiveExpressionNode(ASTNode node, CompilerContext *ctx);
  void visitMultiplicativeNode(ASTNode node, CompilerContext *ctx);
  void visitBitwiseExprNode(ASTNode node, CompilerContext *ctx);
  void visitUnaryExprNode(ASTNode node, CompilerContext *ctx);
  void visitBooleanExprNode(ASTNode node, CompilerContext *ctx);
  void visitSingleBooleanExprNode(ASTNode node, CompilerContext *ctx);
  void visitNullStatementNode(ASTNode node, CompilerContext *ctx);
  std::unordered_map<unsigned int,void (*)(ASTNode node, CompilerContext*)> visitDictionary;
  void constructVisitDictionary ();
}; // class ASTWalker




}; // namespace MAlice

#endif