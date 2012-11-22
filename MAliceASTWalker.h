#include "MAliceLexer.h"
#include "MAliceParser.h"
#include "MAliceSymbolTable.h"

typedef ASTNode MAliceParser_Ctx_struct;

class MAliceASTWalker {
public:
  MAliceASTWalker ();
private:
  void visitCompoundNode(ASTNode*, MALiceSymbolTableNode*);
  void visitProgramNode(ASTNode*, MALiceSymbolTableNode*);
  void visitTypeNode(ASTNode*, MALiceSymbolTableNode*);
  void visitConstantNode(ASTNode*, MALiceSymbolTableNode*);
  void visitFunctionNode(ASTNode*, MALiceSymbolTableNode*);
  void visitProcedureNode(ASTNode*, MALiceSymbolTableNode*);
  void visitDeclarationArgumentListNode(ASTNode*, MALiceSymbolTableNode*);
  void visitBlockNode(ASTNode*, MALiceSymbolTableNode*);
  void visitBodyNode(ASTNode*, MALiceSymbolTableNode*);
  void visitDeclarationArgumentNode(ASTNode*, MALiceSymbolTableNode*);
  void visitFuncInvocationNode(ASTNode*, MALiceSymbolTableNode*);
  void visitAssignmentNode(ASTNode*, MALiceSymbolTableNode*);
  void visitStatementInnerSeperatorNode(ASTNode*, MALiceSymbolTableNode*);
  void visitStatementListNode(ASTNode*, MALiceSymbolTableNode*);
  void visitStatementNode(ASTNode*, MALiceSymbolTableNode*);
  void visitReturnNode(ASTNode*, MALiceSymbolTableNode*);
  void visitWhileLoopNode(ASTNode*, MALiceSymbolTableNode*);
  void visitIfBlockNode(ASTNode*, MALiceSymbolTableNode*);
  void visitElseBlockNode(ASTNode*, MALiceSymbolTableNode*);
  void visitVariableDeclarationNode(ASTNode*, MALiceSymbolTableNode*);
  void visitPrintStatementNode(ASTNode*, MALiceSymbolTableNode*);
  void visitInputStatementNode(ASTNode*, MALiceSymbolTableNode*);
  void visitSTDOUTLvalueNode(ASTNode*, MALiceSymbolTableNode*);
  void visitIncrementNode(ASTNode*, MALiceSymbolTableNode*);
  void visitDecrementNode(ASTNode*, MALiceSymbolTableNode*);
  void visitExpressionNode(ASTNode*, MALiceSymbolTableNode*);
  void visitLvalueNode(ASTNode*, MALiceSymbolTableNode*);
  void visitAdditiveExpressionNode(ASTNode*, MALiceSymbolTableNode*);
  void visitMultiplicativeNode(ASTNode*, MALiceSymbolTableNode*);
  void visitBitwiseExprNode(ASTNode*, MALiceSymbolTableNode*);
  void visitUnaryExprNode(ASTNode*, MALiceSymbolTableNode*);
  void visitBooleanExprNode(ASTNode*, MALiceSymbolTableNode*);
  void visitSingleBooleanExprNode(ASTNode*, MALiceSymbolTableNode*);
  void visitNullStatementNode(ASTNode*, MALiceSymbolTableNode*);
}