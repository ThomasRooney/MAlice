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
  SymbolTable *rootSymbolTable;

private:
  void visitNode(pANTLR3_BASE_TREE node, SymbolTable* symbolTable);
  void visitCompoundNode(ASTNode*, SymbolTable*);
  void visitProgramNode(ASTNode*, SymbolTable*);
  void visitTypeNode(ASTNode*, SymbolTable*);
  void visitConstantNode(ASTNode*, SymbolTable*);
  void visitFunctionNode(ASTNode*, SymbolTable*);
  void visitProcedureNode(ASTNode*, SymbolTable*);
  void visitDeclarationArgumentListNode(ASTNode*, SymbolTable*);
  void visitBlockNode(ASTNode*, SymbolTable*);
  void visitBodyNode(ASTNode*, SymbolTable*);
  void visitDeclarationArgumentNode(ASTNode*, SymbolTable*);
  void visitFuncInvocationNode(ASTNode*, SymbolTable*);
  void visitAssignmentNode(ASTNode*, SymbolTable*);
  void visitStatementInnerSeperatorNode(ASTNode*, SymbolTable*);
  void visitStatementListNode(ASTNode*, SymbolTable*);
  void visitStatementNode(ASTNode*, SymbolTable*);
  void visitReturnNode(ASTNode*, SymbolTable*);
  void visitWhileLoopNode(ASTNode*, SymbolTable*);
  void visitIfBlockNode(ASTNode*, SymbolTable*);
  void visitElseBlockNode(ASTNode*, SymbolTable*);
  void visitVariableDeclarationNode(ASTNode*, SymbolTable*);
  void visitPrintStatementNode(ASTNode*, SymbolTable*);
  void visitInputStatementNode(ASTNode*, SymbolTable*);
  void visitSTDOUTLvalueNode(ASTNode*, SymbolTable*);
  void visitIncrementNode(ASTNode*, SymbolTable*);
  void visitDecrementNode(ASTNode*, SymbolTable*);
  void visitExpressionNode(ASTNode*, SymbolTable*);
  void visitLvalueNode(ASTNode*, SymbolTable*);
  void visitAdditiveExpressionNode(ASTNode*, SymbolTable*);
  void visitMultiplicativeNode(ASTNode*, SymbolTable*);
  void visitBitwiseExprNode(ASTNode*, SymbolTable*);
  void visitUnaryExprNode(ASTNode*, SymbolTable*);
  void visitBooleanExprNode(ASTNode*, SymbolTable*);
  void visitSingleBooleanExprNode(ASTNode*, SymbolTable*);
  void visitNullStatementNode(ASTNode*, SymbolTable*);
}; // class ASTWalker
    
}; // namespace MAlice

#endif