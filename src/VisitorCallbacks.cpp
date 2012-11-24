#include "VisitorCallbacks.h"

#include "FunctionEntity.h"
#include "ProcedureEntity.h"
#include "VariableEntity.h"
#include "Utilities.h"

namespace MAlice {

    // Statements
    void visitNullStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitPrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitStatementListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    // Expressions
    void visitLogicalAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitLogicalOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitLogicalNotExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBitwiseAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBitwiseOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBitwiseXorExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitDivideExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitEqualsExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitGreaterThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitGreaterThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitIdentifierNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitLessThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitLessThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitMinusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitModuloExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitMultiplyExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitNotEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitPlusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitTildeExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    // Imaginary nodes – used to improve AST structure
    void visitArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitArraySubscriptNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBodyNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
        
        walker->visitChildren(node, ctx);
        
        ctx->exitScope();
    }
    
    void visitByReferenceParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitByValueParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitDeclarationsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        walker->visitChildren(node, ctx);
    }
    
    void visitArithmeticExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBooleanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier((char*)identifierNode->toString(identifierNode)->chars);
            
            if (ctx->isSymbolInScope(identifier, NULL)) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                     Utilities::getNodeColumnIndex(identifierNode),
                                                     ErrorTypeSemantic,
                                                     "Symbol " + identifier + " has already been declared in the current scope",
                                                     true);
            }
            
            ctx->addEntityInScope(identifier, FunctionEntity(identifier, NULL, NULL));
        }
        
        walker->visitChildren(node, ctx);
    }
    
    void visitProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitParamsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier((char*)identifierNode->toString(identifierNode)->chars);
            
            if (ctx->isSymbolInScope(identifier, NULL)) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                     Utilities::getNodeColumnIndex(identifierNode),
                                                     ErrorTypeSemantic,
                                                     "Symbol " + identifier + " has already been declared in the current scope.",
                                                     true);
            }
            
            ctx->addEntityInScope(identifier, ProcedureEntity(identifier, NULL));
        }
        
        walker->visitChildren(node, ctx);
    }
    
    void visitVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier((char*)identifierNode->toString(identifierNode)->chars);
            
            if (ctx->isSymbolInScope(identifier, NULL)) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                     Utilities::getNodeColumnIndex(identifierNode),
                                                     ErrorTypeSemantic,
                                                     "Symbol " + identifier + " has already been declared in the current scope.",
                                                     true);
            }
            
            ctx->addEntityInScope(identifier, VariableEntity(identifier, NULL));
        }
    }
    
    // Literals
    void visitCharacterLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitNumberLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitStringLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    // Types
    void visitLetterTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitNumberTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitSentenceTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }


};