#include <string>
#include <sstream>
#include <list>
#include <typeinfo>

#include "VisitorCallbacks.h"
#include "ArrayEntity.h"
#include "Entity.h"
#include "ErrorFactory.h"
#include "FunctionEntity.h"
#include "ProcedureEntity.h"
#include "SemanticChecks.h"
#include "VariableEntity.h"
#include "Utilities.h"
#include "Validation.h"

namespace llvm{}
using namespace llvm;

namespace MAlice {

    bool visitArbitraryBlockNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
    
        bool result = walker->visitChildren(node, NULL, ctx);

        ctx->exitScope();
        
        return result;
    }

    bool visitArithmeticExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitArrayDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateArrayDeclarationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitArraySubscriptNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitAssignmentStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateAssignmentStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitBitwiseAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitBitwiseOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitBitwiseXorExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitBodyNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
        Validation::checkReturnValueForAllExecutionPaths(node, walker, ctx); // Generates warning, carry on.
        
        bool result = walker->visitChildren(node, NULL, ctx);
        
        ctx->exitScope();
        
        return result;
    }
    
    bool visitByReferenceParameterNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitByValueParameterNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitCharacterLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::stringstream strVal;
        strVal.str(Utilities::getNodeText(node));   

        uint64_t val;
        strVal >> val;
        
        APInt ConstructedASM = APInt(
            1, // 1 Byte Long
            val, // Value
            false // Unsigned
            );
        if (outValue)
            *outValue = ConstantInt::get(getGlobalContext(), ConstructedASM);
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitDeclarationsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitDecrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateDecrementStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitDivideExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitEqualsExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->beginExpression();

        bool result = walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), outValue, ctx);

        ctx->endExpression();
        
        return result;
    }

    bool visitFunctionDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateFunctionDeclarationNode(node, walker, ctx))
            return false;
        
        // We've already validated that there is an identifier node here, so this won't be NULL.
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        bool hasParams = false;
        
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            hasParams = true;
        ASTNode returnNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
        MAliceType returnType = Utilities::getTypeFromTypeString(Utilities::getNodeText(returnNode));
        
        FunctionEntity *functionEntity = new FunctionEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::list<ParameterEntity>(), returnType);
        ctx->addEntityInScope(identifier, functionEntity);
        ctx->pushFunctionProcedureEntity(functionEntity);
        
        // Create the llvm::Function and insert it into the module
        std::vector<Type*> parameterTypes;
        std::list<ParameterEntity> parameterEntities = functionEntity->getParameterListTypes();
        
        for (auto it = parameterEntities.begin(); it != parameterEntities.end(); ++it) {
            ParameterEntity entity = *it;
            parameterTypes.push_back(Utilities::getLLVMTypeFromMAliceType(entity.getType()));
        }
        
        ArrayRef<Type*> parameterArrayRefs = makeArrayRef(parameterTypes);
        FunctionType *functionType = FunctionType::get(Utilities::getLLVMTypeFromMAliceType(functionEntity->getReturnType()),
                                                       parameterArrayRefs,
                                                       false);
        
        Function *function = Function::Create(functionType,
                                              Function::InternalLinkage,
                                              identifier.c_str(),
                                              ctx->getModule());
        
        // Walk through the children
        bool result = true;
//        bool result = walker->visitChildren(node, NULL, ctx);
        
        ctx->popFunctionProcedureEntity();
        
        if (!result) {
            // Remove the function from the module it's a part of.
            function->removeFromParent();
            return false;
        }
        
        if (outValue)
            *outValue = function;
        
        return true;
    }

    bool visitGreaterThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitIdentifierNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitIfStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateIfStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitIncrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateIncrementStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }    

    bool visitInputStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateInputStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitLessThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitLessThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitLetterTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitLogicalAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitLogicalNotExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitLogicalOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitMinusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitModuloExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitMultiplyExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitNotEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitNullStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitNumberLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::stringstream strVal;
        strVal.str(Utilities::getNodeText(node));   

        uint64_t val;
        strVal >> val;
        
        APInt ConstructedASM = APInt(
            4, // 4 Bytes Long
            val, // Value
            true // signed
            );
        if (outValue)
            *outValue = ConstantInt::get(getGlobalContext(), ConstructedASM);
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitNumberTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitParamsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *entity = ctx->getCurrentFunctionProcedureEntity();
        std::list<ParameterEntity> parameterList = Utilities::getParameterTypesFromParamsNode(node);
        
        for (auto p = parameterList.begin(); p!=  parameterList.end();p++) {
            if (p->isPassedByReference())
                ctx->addEntityInScope(p->getIdentifier(), new ArrayEntity(p->getIdentifier(), p->getLineNumber(),p->getType(), 1));
            else
                ctx->addEntityInScope(p->getIdentifier(), p->clone());
        }
        
        entity->setParameterListTypes(parameterList);
        return true;
    }

    bool visitPlusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitProcedureDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateProcedureDeclarationNode(node, walker, ctx))
            return false;
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ProcedureEntity *procedureEntity = new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::list<ParameterEntity>());
        ctx->addEntityInScope(identifier, procedureEntity);
        ctx->pushFunctionProcedureEntity(procedureEntity);
        
        // Create the llvm::Function for the procedure and add it to the llvm::Module.
        std::vector<Type*> parameterTypes;
        std::list<ParameterEntity> parameterEntities = procedureEntity->getParameterListTypes();
        
        for (auto it = parameterEntities.begin(); it != parameterEntities.end(); ++it) {
            ParameterEntity entity = *it;
            parameterTypes.push_back(Utilities::getLLVMTypeFromMAliceType(entity.getType()));
        }
        
        ArrayRef<Type*> parameterArrayRefs = makeArrayRef(parameterTypes);
        FunctionType *procedureType = FunctionType::get(Type::getVoidTy(getGlobalContext()),
                                                        parameterArrayRefs,
                                                        false);
        
        Function *procedure = Function::Create(procedureType,
                                               Function::InternalLinkage,
                                               identifier.c_str(),
                                               ctx->getModule());
        
        BasicBlock *block = BasicBlock::Create(getGlobalContext(), "entry", procedure);
        ctx->getIRBuilder()->SetInsertPoint(block);
        
        bool result = walker->visitChildren(node, NULL, ctx);
        if (!result) {
            // Remove the procedure from the Module it's a part of.
            procedure->removeFromParent();
            return false;
        }
        
        ctx->popFunctionProcedureEntity();
        
        // Create the return value for the function, which will exit the scope for the function.
        
        if (outValue)
            *outValue = procedure;
        
        return true;
    }

    bool visitProcFuncInvocationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateProcFuncInvocationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitProgramNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitPrintStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validatePrintStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);

    }

    bool visitReturnStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateReturnStatementNode(node, walker, ctx))
            return false;

        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitSentenceTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitStatementListNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitStringLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitTildeExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitVariableDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateVariableDeclarationNode(node, walker, ctx))
            return false;
        
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitWhileStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateWhileStatementNode(node, walker, ctx))
            return false;
        
        BasicBlock *currentBlock = ctx->getIRBuilder()->GetInsertBlock();
        Function *parentFunction = currentBlock->getParent();
        
        llvm::Value *conditionValue = NULL;
        if (!walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &conditionValue, ctx))
            return false;
        
        BasicBlock *loopHeaderBlock = ctx->getIRBuilder()->GetInsertBlock();
        BasicBlock *loopBody = BasicBlock::Create(getGlobalContext(), "loop", parentFunction);
        ctx->getIRBuilder()->SetInsertPoint(loopBody);
        
        BasicBlock *afterLoopBodyBlock = BasicBlock::Create(getGlobalContext(), "afterloop", parentFunction);
        
        ctx->getIRBuilder()->SetInsertPoint(loopHeaderBlock);
        ctx->getIRBuilder()->CreateCondBr(conditionValue, loopBody, afterLoopBodyBlock);
        ctx->getIRBuilder()->SetInsertPoint(afterLoopBodyBlock);

        return true;
    }
};