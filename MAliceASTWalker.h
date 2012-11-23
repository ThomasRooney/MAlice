#ifndef _MALICEASTWALKER
#define _MALICEASTWALKER

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include <vector>

#include "MAliceSymbolTable.h"


typedef MAliceParser_Ctx_struct ASTNode;

#include "MAliceParser.h"

class MAliceASTWalker {
public:
  MAliceASTWalker ();
  void feedTree();
  bool validateTree(pANTLR3_BASE_TREE);
  void compileTree();
  pANTLR3_STRING printTree(pANTLR3_BASE_TREE, int);
  void initialiseSymbolTable();
  MAliceASTWalker :: ~MAliceASTWalker();
protected:
  MAliceSymbolTableNode *rootSymbolTable;

private:
  void visitNode(pANTLR3_BASE_TREE node, MAliceSymbolTableNode* symbolTable);
  void visitCompoundNode(ASTNode*, MAliceSymbolTableNode*);
  void visitProgramNode(ASTNode*, MAliceSymbolTableNode*);
  void visitTypeNode(ASTNode*, MAliceSymbolTableNode*);
  void visitConstantNode(ASTNode*, MAliceSymbolTableNode*);
  void visitFunctionNode(ASTNode*, MAliceSymbolTableNode*);
  void visitProcedureNode(ASTNode*, MAliceSymbolTableNode*);
  void visitDeclarationArgumentListNode(ASTNode*, MAliceSymbolTableNode*);
  void visitBlockNode(ASTNode*, MAliceSymbolTableNode*);
  void visitBodyNode(ASTNode*, MAliceSymbolTableNode*);
  void visitDeclarationArgumentNode(ASTNode*, MAliceSymbolTableNode*);
  void visitFuncInvocationNode(ASTNode*, MAliceSymbolTableNode*);
  void visitAssignmentNode(ASTNode*, MAliceSymbolTableNode*);
  void visitStatementInnerSeperatorNode(ASTNode*, MAliceSymbolTableNode*);
  void visitStatementListNode(ASTNode*, MAliceSymbolTableNode*);
  void visitStatementNode(ASTNode*, MAliceSymbolTableNode*);
  void visitReturnNode(ASTNode*, MAliceSymbolTableNode*);
  void visitWhileLoopNode(ASTNode*, MAliceSymbolTableNode*);
  void visitIfBlockNode(ASTNode*, MAliceSymbolTableNode*);
  void visitElseBlockNode(ASTNode*, MAliceSymbolTableNode*);
  void visitVariableDeclarationNode(ASTNode*, MAliceSymbolTableNode*);
  void visitPrintStatementNode(ASTNode*, MAliceSymbolTableNode*);
  void visitInputStatementNode(ASTNode*, MAliceSymbolTableNode*);
  void visitSTDOUTLvalueNode(ASTNode*, MAliceSymbolTableNode*);
  void visitIncrementNode(ASTNode*, MAliceSymbolTableNode*);
  void visitDecrementNode(ASTNode*, MAliceSymbolTableNode*);
  void visitExpressionNode(ASTNode*, MAliceSymbolTableNode*);
  void visitLvalueNode(ASTNode*, MAliceSymbolTableNode*);
  void visitAdditiveExpressionNode(ASTNode*, MAliceSymbolTableNode*);
  void visitMultiplicativeNode(ASTNode*, MAliceSymbolTableNode*);
  void visitBitwiseExprNode(ASTNode*, MAliceSymbolTableNode*);
  void visitUnaryExprNode(ASTNode*, MAliceSymbolTableNode*);
  void visitBooleanExprNode(ASTNode*, MAliceSymbolTableNode*);
  void visitSingleBooleanExprNode(ASTNode*, MAliceSymbolTableNode*);
  void visitNullStatementNode(ASTNode*, MAliceSymbolTableNode*);
};

#endif