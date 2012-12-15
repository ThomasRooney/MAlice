#ifndef _generateCodeForORCALLBACKS
#define _generateCodeForORCALLBACKS

#include "CompilerContext.h"
#include "ASTWalker.h"
#include "ParameterEntity.h"

class Value;

namespace MAlice {
    
    class FunctionProcedureEntity;
    
    namespace CodeGeneration {
    
        // Statements
        bool generateCodeForNullStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForAssignmentStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForIncrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForDecrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForIfStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForInputStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForPrintStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForReturnStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForStatementListNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForWhileStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        
        // Expressions
        bool generateCodeForLogicalAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForLogicalOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForLogicalNotExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForBitwiseAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForBitwiseOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForBitwiseXorExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForDivideExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForEqualsExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        
        bool generateCodeForGreaterThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForGreaterThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForIdentifierNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForLessThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForLessThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForMinusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForModuloExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForMultiplyExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForNotEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForPlusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForTildeExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        
        // Imaginary nodes – used to improve AST structure
        bool generateCodeForArrayDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForArraySubscriptNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForBodyNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForByReferenceParameterNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForByValueParameterNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForDeclarationsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForArithmeticExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForFunctionDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForProcFuncInvocationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForParamsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForProgramNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForProcedureDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForVariableDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForArbitraryBlockNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        
        // Literals
        bool generateCodeForCharacterLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForNumberLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForStringLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        
        // Types
        bool generateCodeForLetterTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForNumberTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForSentenceTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);

        bool generateCodeForVariableDeclarationNodeAsGlobalVariable(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForVariableDeclarationNodeAsLocalVariable(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
        
        // Helper functions
        bool generateCodeForUnaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, const llvm::Twine&), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForUnaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, const llvm::Twine&, bool, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
        bool generateCodeForBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx);
        
        llvm::Value *getLLVMValueFromLValueNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx);
        llvm::Function *prettyPrintBoolFunction();
        
        void createAllocasForArguments(CompilerContext *ctx);
        bool hasReturnInstruction(llvm::BasicBlock *block);
        
        llvm::Function *createFunctionForEntity(FunctionProcedureEntity *funcProcEntity, CompilerContext *ctx, bool isEntryPoint = false);
        void extractElementsFromNestedFunctionStruct(CompilerContext *ctx);
        void storeElementsIntoNestedFunctionStruct(CompilerContext *ctx);
        
        void handleNestedFunctionBeforeInvocation(FunctionProcedureEntity *funcProcEntity, llvm::Value *structAlloc, CompilerContext *ctx);
        void handleNestedFunctionAfterInvocation(FunctionProcedureEntity *funcProcEntity, llvm::Value *structAlloc, CompilerContext *ctx);
        
        void addInfoForNestedFunctionOrProcedureToEntity(FunctionProcedureEntity *funcProcEntity, CompilerContext *ctx);
        bool generateCodeForFunctionProcedureNode(ASTNode identifierNode, ASTNode paramsNode, ASTNode bodyNode, FunctionProcedureEntity *funcProcEntity, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx);
    }
}

#endif