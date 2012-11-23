#ifndef _MALICEASTWALKER
#define _MALICEASTWALKER

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include <vector>

#include "SymbolTable.h"

namespace MAlice {

typedef MAliceParser_Ctx_struct ASTNode;

#include "MAliceParser.h"

class ASTWalker {
public:
  ASTWalker();
  void feedTree();
  bool validateTree(pANTLR3_BASE_TREE);
  void compileTree();
  pANTLR3_STRING printTree(pANTLR3_BASE_TREE, int);
  void initialiseSymbolTable();
  ~ASTWalker();
protected:
  SymbolTableNode *rootSymbolTable;

private:
  void visitNode(pANTLR3_BASE_TREE node, SymbolTableNode* symbolTable);
  void visitCompoundNode(ASTNode*, SymbolTableNode*);
  void visitProgramNode(ASTNode*, SymbolTableNode*);
  void visitTypeNode(ASTNode*, SymbolTableNode*);
  void visitConstantNode(ASTNode*, SymbolTableNode*);
  void visitFunctionNode(ASTNode*, SymbolTableNode*);
  void visitProcedureNode(ASTNode*, SymbolTableNode*);
  void visitDeclarationArgumentListNode(ASTNode*, SymbolTableNode*);
  void visitBlockNode(ASTNode*, SymbolTableNode*);
  void visitBodyNode(ASTNode*, SymbolTableNode*);
  void visitDeclarationArgumentNode(ASTNode*, SymbolTableNode*);
  void visitFuncInvocationNode(ASTNode*, SymbolTableNode*);
  void visitAssignmentNode(ASTNode*, SymbolTableNode*);
  void visitStatementInnerSeperatorNode(ASTNode*, SymbolTableNode*);
  void visitStatementListNode(ASTNode*, SymbolTableNode*);
  void visitStatementNode(ASTNode*, SymbolTableNode*);
  void visitReturnNode(ASTNode*, SymbolTableNode*);
  void visitWhileLoopNode(ASTNode*, SymbolTableNode*);
  void visitIfBlockNode(ASTNode*, SymbolTableNode*);
  void visitElseBlockNode(ASTNode*, SymbolTableNode*);
  void visitVariableDeclarationNode(ASTNode*, SymbolTableNode*);
  void visitPrintStatementNode(ASTNode*, SymbolTableNode*);
  void visitInputStatementNode(ASTNode*, SymbolTableNode*);
  void visitSTDOUTLvalueNode(ASTNode*, SymbolTableNode*);
  void visitIncrementNode(ASTNode*, SymbolTableNode*);
  void visitDecrementNode(ASTNode*, SymbolTableNode*);
  void visitExpressionNode(ASTNode*, SymbolTableNode*);
  void visitLvalueNode(ASTNode*, SymbolTableNode*);
  void visitAdditiveExpressionNode(ASTNode*, SymbolTableNode*);
  void visitMultiplicativeNode(ASTNode*, SymbolTableNode*);
  void visitBitwiseExprNode(ASTNode*, SymbolTableNode*);
  void visitUnaryExprNode(ASTNode*, SymbolTableNode*);
  void visitBooleanExprNode(ASTNode*, SymbolTableNode*);
  void visitSingleBooleanExprNode(ASTNode*, SymbolTableNode*);
  void visitNullStatementNode(ASTNode*, SymbolTableNode*);
}; // class ASTWalker
    
}; // namespace MAlice

#endif