#ifndef _VISITORCALLBACKS
#define _VISITORCALLBACKS

#include "CompilerContext.h"
#include "ASTWalker.h"
#include "ParameterEntity.h"

class Value;

namespace MAlice {
    
    class FunctionProcedureEntity;
    
    // Statements
    bool visitNullStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitAssignmentStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitIncrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitDecrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitIfStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitInputStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitPrintStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitReturnStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitStatementListNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitWhileStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    
    // Expressions
    bool visitLogicalAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitLogicalOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitLogicalNotExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitBitwiseXorExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitDivideExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitEqualsExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    
    bool visitGreaterThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitIdentifierNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitLessThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitLessThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitMinusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitModuloExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitMultiplyExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitNotEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitPlusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitTildeExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    
    // Imaginary nodes – used to improve AST structure
    bool visitArrayDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitArraySubscriptNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitBodyNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitByReferenceParameterNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitByValueParameterNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitDeclarationsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitArithmeticExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitFunctionDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitProcFuncInvocationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitParamsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitProgramNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitProcedureDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitVariableDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitArbitraryBlockNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    
    // Literals
    bool visitCharacterLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitNumberLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitStringLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    
    // Types
    bool visitLetterTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitNumberTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitSentenceTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);

    bool visitVariableDeclarationNodeAsGlobalVariable(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    bool visitVariableDeclarationNodeAsLocalVariable(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    
    // Helper functions
    bool visitUnaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, const llvm::Twine&), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
    bool visitUnaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, const llvm::Twine&, bool, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
    bool visitBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
    bool visitBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
    
    llvm::Value *getLLVMValueFromLValueNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
}

#endif