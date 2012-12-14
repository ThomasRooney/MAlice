#include <string>
#include <sstream>
#include <list>
#include <typeinfo>

#include "CodeGeneration.h"
#include "Entity.h"
#include "ErrorFactory.h"
#include "FunctionEntity.h"
#include "GlobalVariableEntity.h"
#include "IdentifierDispenser.h"
#include "ProcedureEntity.h"
#include "SemanticChecks.h"
#include "VariableEntity.h"
#include "Utilities.h"
#include "Validation.h"
#include "LLVMHeader.h"

namespace llvm{}
using namespace llvm;

namespace MAlice {

    bool CodeGeneration::generateCodeForArbitraryBlockNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
        ctx->enterDebugScope(node);
        
        bool result = walker->generateCodeForChildren(node, NULL, ctx);

        ctx->exitScope();
        ctx->exitDebugScope(node);
        
        return result;
    }

    bool CodeGeneration::generateCodeForArithmeticExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForArrayDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ASTNode typeNode = Utilities::getChildNodeAtIndex(identifierNode, 0);
        std::string typeString = Utilities::getNodeText(typeNode);
        
        ASTNode numElementsNode = Utilities::getChildNodeAtIndex(identifierNode, 1);
        
        Type arrayType = Utilities::getTypeFromTypeString(typeString);
        arrayType.setIsArray(true);
        VariableEntity *arrayEntity = new VariableEntity(identifier, Utilities::getNodeLineNumber(node), arrayType);
        ctx->addEntityInScope(identifier, arrayEntity);

        llvm::Value *numElementsValue = NULL;
        walker->generateCodeForNode(numElementsNode, &numElementsValue, ctx);
        
        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        // TODO: Generate variable declaration debug information

        llvm::Value *value = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromType(arrayEntity->getType()), numElementsValue);
        arrayEntity->setLLVMValue(value);
        
        return true;
    }

    bool CodeGeneration::generateCodeForArraySubscriptNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ASTNode indexNode = Utilities::getChildNodeAtIndex(node, 1);
        llvm::Value *indexValue = NULL;
        walker->generateCodeForNode(indexNode, &indexValue, ctx);
        
        Entity *entity = NULL;
        ctx->isSymbolInScope(identifier, &entity);
        
        VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
        
        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       


        if (outValue) {
            llvm::Value *pointerValue = ctx->getIRBuilder()->CreateGEP(variableEntity->getLLVMValue(), indexValue);
            *outValue = ctx->getIRBuilder()->CreateLoad(pointerValue);
        }
        
        return true;
    }

    bool CodeGeneration::generateCodeForAssignmentStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *lvalueValue = getLLVMValueFromLValueNode(Utilities::getChildNodeAtIndex(node, 0), walker, ctx);
        llvm::Value *assignmentValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &assignmentValue, ctx);
        
        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       


        if (outValue)
            *outValue = ctx->getIRBuilder()->CreateStore(assignmentValue, lvalueValue);
        
        return true;
    }

    bool CodeGeneration::generateCodeForBitwiseAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                                 outValue,
                                                 &llvm::IRBuilder<>::CreateAnd,
                                                 "andtmp",
                                                 walker,
                                                 ctx);
    }
    
    bool CodeGeneration::generateCodeForBitwiseOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                                 outValue,
                                                 &llvm::IRBuilder<>::CreateOr,
                                                 "andtmp",
                                                 walker,
                                                 ctx);
    }
    
    bool CodeGeneration::generateCodeForBitwiseXorExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                                 outValue,
                                                 &llvm::IRBuilder<>::CreateXor,
                                                 "xortmp",
                                                 walker,
                                                 ctx);
    }

    bool CodeGeneration::generateCodeForBodyNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Function *function = ctx->getCurrentFunctionProcedureEntity()->getLLVMFunction();
        
        BasicBlock *bodyBlock = BasicBlock::Create(getGlobalContext(), "entry", function);
        ctx->getIRBuilder()->SetInsertPoint(bodyBlock);
        
        ctx->enterScope();
        ctx->enterDebugScope(node);
        
        createAllocasForArguments(ctx);
        
        bool result = walker->generateCodeForChildren(node, NULL, ctx);
        
        ctx->exitDebugScope(node);
        ctx->exitScope();
        
        return result;
    }
    
    bool CodeGeneration::generateCodeForByReferenceParameterNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }
    
    bool CodeGeneration::generateCodeForByValueParameterNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForCharacterLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::string strVal = Utilities::getNodeText(node);
        uint64_t val = strVal[1]; // [0] = "'", [1] = LITERAL, [2] = "'"
        
        if (outValue)
            *outValue = ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeLetter)), val);
        
        return true;
    }

    bool CodeGeneration::generateCodeForDeclarationsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForDecrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *lhsValue = getLLVMValueFromLValueNode(Utilities::getChildNodeAtIndex(node, 0),
                                                           walker,
                                                           ctx);

        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        
        llvm::Value *loadValue = ctx->getIRBuilder()->CreateLoad(lhsValue);
        llvm::Value *subValue = ctx->getIRBuilder()->CreateSub(loadValue, ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), 1));
        ctx->getIRBuilder()->CreateStore(subValue, lhsValue);
        
        return true;
    }

    bool CodeGeneration::generateCodeForDivideExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }
    
    bool CodeGeneration::generateCodeForEqualsExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                                 outValue,
                                                 &llvm::IRBuilder<>::CreateICmpEQ,
                                                 "eqtmp",
                                                 walker,
                                                 ctx);
    }

    bool CodeGeneration::generateCodeForExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        bool result;
        ctx->beginExpression();
        // Populate the Expression Code Generation
        
        // We are going to have only one child
        ASTNode childNode = Utilities::getChildNodeAtIndex(node, 0);
        result = walker->generateCodeForNode(childNode, outValue, ctx);
        ctx->endExpression();
        
        return result;
    }

    bool CodeGeneration::generateCodeForFunctionDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        // We've already validated that there is an identifier node here, so this won't be NULL.
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        bool hasParams = false;
        
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            hasParams = true;
        ASTNode returnNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
        
        FunctionEntity *functionEntity = new FunctionEntity(identifier,
                                                            Utilities::getNodeLineNumber(identifierNode),
                                                            std::vector<ParameterEntity*>(),
                                                            Utilities::getTypeFromTypeString(Utilities::getNodeText(returnNode)));
        ctx->addEntityInScope(identifier, functionEntity);
        ctx->pushFunctionProcedureEntity(functionEntity);
        ctx->saveInsertPoint();
        
        if (hasParams) {
            if (!walker->generateCodeForNode(nodeI1, NULL, ctx))
                return false;
        }
        
        llvm::Function *function = createFunctionForEntity(functionEntity, ctx);
        functionEntity->setLLVMFunction(function);
    
        ASTNode bodyNode = Utilities::getChildNodeAtIndex(node, hasParams?3:2);
        bool result = walker->generateCodeForNode(bodyNode, NULL, ctx);
        
        ctx->restoreInsertPoint();
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

    bool CodeGeneration::generateCodeForGreaterThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                                 outValue,
                                                 &llvm::IRBuilder<>::CreateICmpSGT,
                                                 "gttmp",
                                                 walker,
                                                 ctx);
    }
    
    bool CodeGeneration::generateCodeForGreaterThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                                 outValue,
                                                 &llvm::IRBuilder<>::CreateICmpSGE,
                                                 "gtetmp",
                                                 walker,
                                                 ctx);
    }
    
    bool CodeGeneration::generateCodeForIdentifierNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::string identifier = Utilities::getNodeText(node);
        
        Entity *entity = NULL;
        if (!ctx->isSymbolInScope(identifier, &entity))
            return false;
        
        if (!Utilities::isKindOfEntity(entity, MAliceEntityTypeVariable))
            return false;
        
        VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
        llvm::Value *value = variableEntity->getLLVMValue();

        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        llvm::Value *loadInst = ctx->getIRBuilder()->CreateLoad(value);

        if (outValue)
            *outValue = loadInst;
        
        return true;
    }

    bool CodeGeneration::generateCodeForIfStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        Function *function = ctx->getCurrentFunctionProcedureEntity()->getLLVMFunction();
        llvm::IRBuilder<> *builder = ctx->getIRBuilder();
        llvm::BasicBlock *afterBlock = llvm::BasicBlock::Create(getGlobalContext(), "after");
        llvm::BasicBlock *lastBlock = NULL;
        
        bool usesAfterBlock = false;

        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(node); ++i) {
            ASTNode node1 = Utilities::getChildNodeAtIndex(node, i);
            
            if (Utilities::getNodeType(node1) == EXPRESSION) {
                llvm::Value *condValue = NULL;
                walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &condValue, ctx);
                
                llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(getGlobalContext(), "then");
                llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(getGlobalContext(), "else");
                
                builder->CreateCondBr(condValue, thenBlock, elseBlock);
                
                // Change debug line number
                if (ctx->getDGBuilder())
                {
                    builder->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                         Utilities::getNodeColumnIndex(node),
                                                                         ctx->getCurrentDBScope()));
                }
                
                // Generate code for the
                function->getBasicBlockList().push_back(thenBlock);
                builder->SetInsertPoint(thenBlock);
                walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, i+1), NULL, ctx);
                
                if (!hasReturnInstruction(thenBlock)) {
                    builder->CreateBr(afterBlock);
                    usesAfterBlock = true;
                }
                
                // Insert the 'else' block
                function->getBasicBlockList().push_back(elseBlock);
                builder->SetInsertPoint(elseBlock);
                
                // We will want to check whether this block contains a return statement if it is the last block.
                lastBlock = elseBlock;
                
                // We have looked at the 'then' node too.
                i++;
            }
            else
                walker->generateCodeForNode(node1, NULL, ctx);
        }
        
        if (lastBlock && !hasReturnInstruction(lastBlock))
            builder->CreateBr(afterBlock);
        
        if (usesAfterBlock)
            function->getBasicBlockList().push_back(afterBlock);
        builder->SetInsertPoint(afterBlock);
        
        return true;
    }

    bool CodeGeneration::generateCodeForIncrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *lhsValue = getLLVMValueFromLValueNode(Utilities::getChildNodeAtIndex(node, 0), walker, ctx);
        
        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       


        ctx->getIRBuilder()->CreateAdd(lhsValue, ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), 1));
        
        return true;
    }

    bool CodeGeneration::generateCodeForInputStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }
        
        llvm::Value *inputVal = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &inputVal, ctx);

        // We have to be clever here, as we need a *pointer* to use with scanf().
        llvm::Value *value = getLLVMValueFromLValueNode(Utilities::getChildNodeAtIndex(node, 0), walker, ctx);
        
        Type type;
        Utilities::getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0),
                                             &type,
                                             false,
                                             walker,
                                             ctx,
                                             NULL);
        
        if (type == Type(PrimitiveTypeSentence)) {
            llvm::Value *callValue = ctx->getIRBuilder()->CreateCall(Utilities::getCallocFunction(ctx->getModule()), llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), 1024));
            ctx->getIRBuilder()->CreateStore(callValue, value);
        }
        
        llvm::Value *formatStringValue = ctx->ioFormatStringForExpressionType(type);
        
        // Create the scanf() call.
        ctx->getIRBuilder()->CreateCall2(Utilities::getScanfFunction(ctx->getModule()), formatStringValue, value);
        
        return true;
    }

    bool CodeGeneration::generateCodeForLessThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                                 outValue,
                                                 &llvm::IRBuilder<>::CreateICmpSLT,
                                                 "lttemp",
                                                 walker,
                                                 ctx);
    }
    
    bool CodeGeneration::generateCodeForLessThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateICmpSLE,
                                       "ltetmp",
                                       walker,
                                       ctx);
    }
    
    bool CodeGeneration::generateCodeForBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(leftParamValue, rightParamValue, twine);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
    }
    
    bool CodeGeneration::generateCodeForBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(leftParamValue, rightParamValue, twine, false, false);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
    }
    
    bool CodeGeneration::generateCodeForUnaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, const llvm::Twine&), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *paramValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &paramValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(paramValue, twine);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
    }
    
    bool CodeGeneration::generateCodeForUnaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, const llvm::Twine&, bool, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *paramValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &paramValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(paramValue, twine, false, false);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
    }

    bool CodeGeneration::generateCodeForLetterTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForLogicalAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Function *function = ctx->getCurrentFunctionProcedureEntity()->getLLVMFunction();
        llvm::IRBuilder<> *builder = ctx->getIRBuilder();

        llvm::BasicBlock *leftAndBlock = llvm::BasicBlock::Create(getGlobalContext(), "and_left", function);
        builder->CreateBr(leftAndBlock);
        builder->SetInsertPoint(leftAndBlock);
        
        llvm::Value *leftExpressionValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &leftExpressionValue, ctx);
        
        llvm::BasicBlock *rightAndBlock = llvm::BasicBlock::Create(getGlobalContext(), "and_right");
        llvm::BasicBlock *afterAndBlock = llvm::BasicBlock::Create(getGlobalContext(), "and_after");
        
        // Create the conditional branch instruction to implement left-strictness.
        builder->CreateCondBr(leftExpressionValue, rightAndBlock, afterAndBlock);
        
        // Generate code for the right expression to the && operation
        llvm::Value *rightExpressionValue = NULL;
        builder->SetInsertPoint(rightAndBlock);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &rightExpressionValue, ctx);
        
        // LLVM IR requires that each block is terminated by a branch or return instruction
        builder->CreateBr(afterAndBlock);
        
        // Add the new blocks to the function
        function->getBasicBlockList().push_back(rightAndBlock);
        function->getBasicBlockList().push_back(afterAndBlock);
        
        builder->SetInsertPoint(afterAndBlock);
        
        llvm::PHINode *phiNode = ctx->getIRBuilder()->CreatePHI(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeBoolean)), 2,
                                                                "andtmp");
        
        phiNode->addIncoming(leftExpressionValue, leftAndBlock);
        phiNode->addIncoming(rightExpressionValue, rightAndBlock);
        
        if (outValue)
            *outValue = phiNode;
        
        return true;
    }
    
    bool CodeGeneration::generateCodeForLogicalNotExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *expressionValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &expressionValue, ctx);
        
        return generateCodeForUnaryOperatorNode(node,
                                      outValue,
                                      &llvm::IRBuilder<>::CreateNot,
                                      "nottmp",
                                      walker,
                                      ctx);
    }

    bool CodeGeneration::generateCodeForLogicalOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Function *function = ctx->getCurrentFunctionProcedureEntity()->getLLVMFunction();
        
        llvm::BasicBlock *leftOrBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "or_left", function);
        llvm::BasicBlock *rightOrBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "or_right");
        llvm::BasicBlock *afterOrBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "or_after");

        llvm::IRBuilder<> *builder = ctx->getIRBuilder();
        builder->CreateBr(leftOrBlock);
        builder->SetInsertPoint(leftOrBlock);
        
        llvm::Value *leftExpressionValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &leftExpressionValue, ctx);
        
        // Create the conditional branch which means we skip the right expression evaluation if the left expression evaluates to true.
        builder->CreateCondBr(leftExpressionValue, afterOrBlock, rightOrBlock);
        
        // Generate code for the right expression
        llvm::Value *rightExpressionValue = NULL;
        builder->SetInsertPoint(rightOrBlock);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &rightExpressionValue, ctx);
        
        // LLVM IR requires that each block is terminated by a branch or return instruction
        builder->CreateBr(afterOrBlock);
        
        // Add the blocks to the function
        function->getBasicBlockList().push_back(rightOrBlock);
        function->getBasicBlockList().push_back(afterOrBlock);
        
        builder->SetInsertPoint(afterOrBlock);
        
        llvm::PHINode *phiNode = ctx->getIRBuilder()->CreatePHI(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeBoolean)), 2,
                                                                "ortmp");
        
        phiNode->addIncoming(leftExpressionValue, leftOrBlock);
        phiNode->addIncoming(rightExpressionValue, rightOrBlock);
        
        if (outValue)
            *outValue = phiNode;
        
        return true;
    }
    
    bool CodeGeneration::generateCodeForMinusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) == 1)
            return generateCodeForUnaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateNeg, "negtmp", walker, ctx);
        
        return generateCodeForBinaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateSub, "subtmp", walker, ctx);
    }
    
    bool CodeGeneration::generateCodeForModuloExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node, outValue,
                                       &llvm::IRBuilder<>::CreateSRem,
                                       "modtmp",
                                       walker,
                                       ctx);
    }
    
    bool CodeGeneration::generateCodeForMultiplyExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateMul,
                                       "multmp",
                                       walker,
                                       ctx);
    }
    
    bool CodeGeneration::generateCodeForNotEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateICmpEQ,
                                       "netmp",
                                       walker,
                                       ctx);
    }

    bool CodeGeneration::generateCodeForNullStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return true;
    }

    bool CodeGeneration::generateCodeForNumberLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::stringstream strVal;
        strVal.str(Utilities::getNodeText(node));   

        int64_t val;
        strVal >> val;
        
        if (outValue)
            *outValue = ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), val);
        
        return true;
    }

    bool CodeGeneration::generateCodeForNumberTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForParamsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        FunctionProcedureEntity *entity = ctx->getCurrentFunctionProcedureEntity();
        std::vector<ParameterEntity*> parameterList = Utilities::getParameterTypesFromParamsNode(node);
        
        for (auto it = parameterList.begin(); it != parameterList.end(); ++it) {
            ParameterEntity *entity = *it;
            ctx->addEntityInScope(entity->getIdentifier(), entity);
        }
        
        entity->setParameterListTypes(parameterList);
        
        return true;
    }

    bool CodeGeneration::generateCodeForPlusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) == 1) {
            // The unary + operator does nothing!
            return true;
        }
        
        return generateCodeForBinaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateSub, "addtmp", walker, ctx);
    }

    bool CodeGeneration::generateCodeForProcedureDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        bool hasParams = false;
        
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            hasParams = true;
        
        bool isEntryPointProcedure = !ctx->getCurrentFunctionProcedureEntity() && identifier == "hatta";
        
        ProcedureEntity *procedureEntity = new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::vector<ParameterEntity*>());
        
        if (ctx->getCurrentFunctionProcedureEntity())
            ctx->saveInsertPoint();
        
        ctx->addEntityInScope(identifier, procedureEntity);
        ctx->pushFunctionProcedureEntity(procedureEntity);
        
        if (hasParams) {
            if (!walker->generateCodeForNode(nodeI1, NULL, ctx))
                return false;
        }
        
        llvm::Function *function = createFunctionForEntity(procedureEntity, ctx, isEntryPointProcedure);
        procedureEntity->setLLVMFunction(function);
        
        ASTNode bodyNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
        bool result = walker->generateCodeForNode(bodyNode, NULL, ctx);
        
        // LLVM requires all functions to return a value
        ctx->getIRBuilder()->CreateRetVoid();
        
        ctx->restoreInsertPoint();
        ctx->popFunctionProcedureEntity();
        
        if (!result) {
            // Remove the function from the module it's a part of.
            function->removeFromParent();
            return false;
        }
        
        return true;
    }

    bool CodeGeneration::generateCodeForPrintStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {        
        Function *printfFunction = Utilities::getPrintfFunction(ctx->getModule());
        
        Type type;
        Utilities::getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0),
                                             &type,
                                             false,
                                             walker,
                                             ctx,
                                             NULL);
        
        llvm::Value *printVal = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &printVal, ctx);
        
        if (type.getPrimitiveType() == PrimitiveTypeBoolean) {
            ctx->getIRBuilder()->CreateCall(ctx->prettyPrintBoolFunction(), printVal);
            return true;
        }
        
        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        llvm::Value *formatStringValue = ctx->ioFormatStringForExpressionType(type);
        // Create the printf() call.
        ctx->getIRBuilder()->CreateCall2(printfFunction, formatStringValue, printVal);
        
        return true;
    }

    bool CodeGeneration::generateCodeForProcFuncInvocationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        Entity *entity = NULL;
        if (!ctx->isSymbolInScope(identifier, &entity))
            return false;
        
        std::vector<llvm::Value*> arguments;

        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(identifierNode); ++i) {
            llvm::Value *value = NULL;
            walker->generateCodeForNode(Utilities::getChildNodeAtIndex(identifierNode, i), &value, ctx);
            arguments.push_back(value);
        }
        
        FunctionProcedureEntity *funcProcEntity = dynamic_cast<FunctionProcedureEntity*>(entity);
        llvm::ArrayRef<llvm::Value*> llvmArguments = llvm::makeArrayRef(arguments);

        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        if (Utilities::getTypeOfEntity(funcProcEntity) == MAliceEntityTypeFunction) {
            llvm::Value * v = ctx->getIRBuilder()->CreateCall(funcProcEntity->getLLVMFunction(), llvmArguments, "calltmp");
            if (outValue)
                *outValue = v;
        } else {
            ctx->getIRBuilder()->CreateCall(funcProcEntity->getLLVMFunction(), llvmArguments);
        }

        return true;
    }

    bool CodeGeneration::generateCodeForProgramNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (ctx->getDGBuilder())
            // Create a lexical lock node to declare DWARF debug information scope
            ctx->enterDebugScope(node);


        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForReturnStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *returnValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &returnValue, ctx);

        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }

        ctx->getIRBuilder()->CreateRet(returnValue);

        return true;
    }

    bool CodeGeneration::generateCodeForSentenceTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForStatementListNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForStringLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::string strVal = Utilities::getNodeText(node);
        strVal = Utilities::stripLeadingAndTrailingCharacters(strVal, '"');

        if (outValue)
            *outValue = ctx->getIRBuilder()->CreateGlobalStringPtr(Utilities::stringWithASCIIControlCodes(strVal.c_str()));
        
        return true;
    }

    bool CodeGeneration::generateCodeForTildeExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForUnaryOperatorNode(node,
                                                outValue,
                                                &llvm::IRBuilder<>::CreateNot,
                                                "nottmp",
                                                walker,
                                                ctx);
    }

    bool CodeGeneration::generateCodeForVariableDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!ctx->getCurrentFunctionProcedureEntity())
            return generateCodeForVariableDeclarationNodeAsGlobalVariable(node, outValue, walker, ctx);
        
        return generateCodeForVariableDeclarationNodeAsLocalVariable(node, outValue, walker, ctx);
    }
    
    bool CodeGeneration::generateCodeForVariableDeclarationNodeAsGlobalVariable(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        std::string typeString = Utilities::getNodeText(typeNode);
        
        GlobalVariableEntity *variable = new GlobalVariableEntity(identifier, Utilities::getNodeLineNumber(identifierNode), Utilities::getTypeFromTypeString(typeString));
        
        llvm::Type *type = Utilities::getLLVMTypeFromType(variable->getType());
        llvm::Constant *initialiserValue = llvm::ConstantInt::get(type, 0);
        ASTNode valueNode = Utilities::getChildNodeAtIndex(node, 2);
        
        if (valueNode) {
            llvm::Value *value = NULL;
            walker->generateCodeForNode(valueNode, &value, ctx);
            
            llvm::Constant *constant = llvm::cast<llvm::Constant>(value);
            
            if (constant)
                initialiserValue = constant;
        }
        
        llvm::GlobalVariable *value = new GlobalVariable(*(ctx->getModule()),
                                                         type,
                                                         false,
                                                         GlobalValue::InternalLinkage,
                                                         initialiserValue,
                                                         Twine(identifier.c_str()));
        
        variable->setLLVMValue(value);
        ctx->addEntityInScope(identifier, variable);
        
        // Add debug info
        if (ctx->getDGBuilder())
        {
            llvm::DIGlobalVariable debugVar = ctx->getDGBuilder()->createGlobalVariable(identifier,
                                                    *ctx->getDIFile(),
                                                    Utilities::getNodeLineNumber(node),
                                                    llvm::DIType(), // TODO: Btter type information so it can be displayed in gdb
                                                    false,
                                                    value);
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       


        if (outValue)
            *outValue = ctx->getIRBuilder()->CreateLoad(value);
            
        return true;
    }
    
    bool CodeGeneration::generateCodeForVariableDeclarationNodeAsLocalVariable(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        std::string typeString = Utilities::getNodeText(typeNode);
        
        ASTNode valueNode = Utilities::getChildNodeAtIndex(node, 2);
        
        VariableEntity *variable = new VariableEntity(identifier,
                                                      Utilities::getNodeLineNumber(identifierNode),
                                                      Utilities::getTypeFromTypeString(typeString));
        if (ctx->getDGBuilder())
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));


        llvm::Value *value = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromType(variable->getType()), NULL, identifier.c_str());
        variable->setLLVMValue(value);
        ctx->addEntityInScope(identifier, variable);

        // Add debug info
        if (ctx->getDGBuilder())
        {
            llvm::DIVariable debugVar = ctx->getDGBuilder()->createLocalVariable(llvm::dwarf::DW_TAG_auto_variable,
                                                    llvm::DIDescriptor(ctx->getCurrentDBScope()),
                                                    identifier,
                                                    *ctx->getDIFile(),
                                                    Utilities::getNodeLineNumber(node),
                                                    llvm::DIType(), // TODO: Btter type information so it can be displayed in gdb
                                                    true);
            ctx->getDGBuilder()->insertDeclare(value, debugVar, ctx->getIRBuilder()->GetInsertBlock());
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));

        }                                       
        
        if (valueNode) {
            llvm::Value *assignmentValue = NULL;
            if (walker->generateCodeForNode(valueNode, &assignmentValue, ctx))
                ctx->getIRBuilder()->CreateStore(assignmentValue, value);
        }
        
        if (outValue)
            *outValue = value;
        
        return true;
    }
    
    bool CodeGeneration::generateCodeForWhileStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Function *function = ctx->getCurrentFunctionProcedureEntity()->getLLVMFunction();
        llvm::IRBuilder<> *builder = ctx->getIRBuilder();
        
        llvm::Value *conditionValue = NULL;
        if (!walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &conditionValue, ctx))
            return false;
        
        llvm::BasicBlock *loopHeader = llvm::BasicBlock::Create(getGlobalContext(), "loopheader", function);
        builder->CreateBr(loopHeader);
        
        llvm::BasicBlock *loopBody = llvm::BasicBlock::Create(getGlobalContext(), "loopbody");
        llvm::BasicBlock *afterLoopBlock = llvm::BasicBlock::Create(getGlobalContext(), "afterloop");
        
        llvm::Value *exitConditionValue = NULL;
        builder->SetInsertPoint(loopHeader);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &exitConditionValue, ctx);
        
        // Exit the loop if the exit condition is satisfied
        builder->CreateCondBr(exitConditionValue, afterLoopBlock, loopBody);
        
        // Generate code for the loop body
        function->getBasicBlockList().push_back(loopBody);
        builder->SetInsertPoint(loopBody);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), NULL, ctx);
        builder->CreateBr(loopHeader);
        
        function->getBasicBlockList().push_back(afterLoopBlock);
        
        builder->SetInsertPoint(afterLoopBlock);

        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }

        return true;
    }
    
    llvm::Value *CodeGeneration::getLLVMValueFromLValueNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNodeType(node) == EXPRESSION)
            node = Utilities::getChildNodeAtIndex(node, 0);
        
        ANTLR3_UINT32 type = Utilities::getNodeType(node);
        
        if (type != IDENTIFIER && type != ARRAYSUBSCRIPT)
            return NULL;
        
        // An l-value for an expression can only be an identifier
        if (type == IDENTIFIER) {
            Entity *entity = NULL;
            ctx->isSymbolInScope(Utilities::getNodeText(node), &entity);
            
            VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
            if (!variableEntity)
                return NULL;
            
            return variableEntity->getLLVMValue();
        }
        
        llvm::Value *elementValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &elementValue, ctx);
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        if (!identifierNode)
            return NULL;
        
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        Entity *entity = NULL;
        ctx->isSymbolInScope(Utilities::getNodeText(identifierNode), &entity);
        
        VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
        if (!variableEntity)
            return NULL;
        
        return ctx->getIRBuilder()->CreateGEP(variableEntity->getLLVMValue(), elementValue);
    }
    
    void CodeGeneration::createAllocasForArguments(CompilerContext *ctx)
    {
        llvm::IRBuilder<> *builder = ctx->getIRBuilder();
        
        FunctionProcedureEntity *funcProcEntity = ctx->getCurrentFunctionProcedureEntity();
        llvm::Function *function = funcProcEntity->getLLVMFunction();
        
        unsigned int i = 0;
        
        for (auto it = function->arg_begin(); it != function->arg_end(); ++it) {
            ParameterEntity *parameterEntity = funcProcEntity->getParameterListTypes().at(i);
            
            llvm::Value *alloca = builder->CreateAlloca(Utilities::getLLVMTypeFromType(parameterEntity->getType()));
            builder->CreateStore(it, alloca);
            parameterEntity->setLLVMValue(alloca);
            
            ++i;
        }
    }
    
    bool CodeGeneration::hasReturnInstruction(llvm::BasicBlock *block)
    {
        for (auto it = block->begin(); it != block->end(); ++it) {
            Instruction *instruction = it;
            
            if (llvm::dyn_cast<llvm::ReturnInst>(instruction))
                return true;
        }
        
        return false;
    }
    
    llvm::Function *CodeGeneration::createFunctionForEntity(FunctionProcedureEntity *funcProcEntity, CompilerContext *ctx, bool isEntryPoint)
    {
        // Create the llvm::Function and insert it into the module
        std::vector<ParameterEntity*> parameterEntities = funcProcEntity->getParameterListTypes();
        std::vector<llvm::Type*> parameterTypes;
        for (auto it = parameterEntities.begin(); it != parameterEntities.end(); ++it) {
            ParameterEntity *entity = *it;
            llvm::Type *llvmType = Utilities::getLLVMTypeFromType(entity->getType());
            
            parameterTypes.push_back(llvmType);
        }
        
        llvm::Type *returnType = llvm::Type::getVoidTy(llvm::getGlobalContext());
        if (Utilities::getTypeOfEntity(funcProcEntity) == MAliceEntityTypeFunction) {
            FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(funcProcEntity);
            returnType = Utilities::getLLVMTypeFromType(functionEntity->getReturnType());
        }
        
        FunctionType *functionType = FunctionType::get(returnType, parameterTypes, false);
        std::string identifier = funcProcEntity->getIdentifier();
        std::string LLVMIdentifier;
        if (isEntryPoint)
            LLVMIdentifier = ctx->getIdentifierDispenser()->identifierForEntryPointFunction();
        else
            LLVMIdentifier = ctx->getIdentifierDispenser()->identifierForFunctionWithName(identifier);
        
        Function *function = Function::Create(functionType,
                                              Function::ExternalLinkage,
                                              LLVMIdentifier,
                                              ctx->getModule());
        
        // Set the arg names in the LLVM function
        for (auto it = function->arg_begin(); it != function->arg_end(); ++it) {
            llvm::Value *arg = it;
            arg->setName("x");
        }
        
        return function;
    }
    
};
