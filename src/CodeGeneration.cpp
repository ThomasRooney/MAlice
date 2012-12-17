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
#include "StringTable.h"

namespace MAlice {

    bool CodeGeneration::generateCodeForArbitraryBlockNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
        ctx->enterDebugScope(node);
        
        bool result = walker->generateCodeForChildren(node, NULL, ctx);

        ctx->exitScope();
        ctx->exitDebugScope();
        
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
        
        llvm::Value *value = NULL;
        
        if (!ctx->getCurrentFunctionProcedureEntity()) {
            int64_t numElements = 0;
            if (!Utilities::extractValueFromExpressionNode(numElementsNode, &numElements, walker, ctx))
                return false;
            
            std::vector<llvm::Constant*> initialiser;
            Type primitiveType(arrayType.getPrimitiveType());
            
            for (int64_t i = 0; i < numElements; ++i) {
                initialiser.push_back(Utilities::llvmDefaultValueForType(primitiveType));
            }
            
            llvm::ArrayType *llvmArrayType = llvm::ArrayType::get(Utilities::getLLVMTypeFromType(primitiveType), numElements);
            
            llvm::Value *globalVariable = new llvm::GlobalVariable(*(ctx->getModule()),
                                                                   llvmArrayType,
                                                                   false,
                                                                   llvm::GlobalVariable::PrivateLinkage,
                                                                   llvm::ConstantArray::get(llvmArrayType, initialiser),
                                                                   "");
            
            std::vector<llvm::Value*> indices;
            indices.push_back(llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), 0));
            indices.push_back(llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), 0));
            
            value = ctx->getIRBuilder()->CreateInBoundsGEP(globalVariable, indices);
        }
        else {
            llvm::Value *numElementsValue = NULL;
            walker->generateCodeForNode(numElementsNode, &numElementsValue, ctx);
            
            Type primitiveType = Type(arrayEntity->getType().getPrimitiveType());
            value = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromType(primitiveType), numElementsValue);
        }

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
        
        if (outValue) {
            
            llvm::Type *containedType = lvalueValue->getType()->getContainedType(0);
            
            // If we're assigning into a by-reference array subscript, we have to ensure the types match
            if (containedType && containedType->isPointerTy()) {
                llvm::Value *tempAlloca = ctx->getIRBuilder()->CreateAlloca(assignmentValue->getType());
                ctx->getIRBuilder()->CreateStore(assignmentValue, tempAlloca);
                
                *outValue = ctx->getIRBuilder()->CreateStore(tempAlloca, lvalueValue);
            } else
                *outValue = ctx->getIRBuilder()->CreateStore(assignmentValue, lvalueValue);
        }
        
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
        
        llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", function);
        ctx->getIRBuilder()->SetInsertPoint(bodyBlock);
        
        createAllocasForArguments(ctx);
        extractElementsFromNestedFunctionStruct(ctx);
        
        ctx->enterDebugScope(node);
        
        bool result = walker->generateCodeForChildren(node, NULL, ctx);
        
        ctx->exitDebugScope();
        
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
            *outValue = llvm::ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeLetter)), val);
        
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
        
        llvm::Value *loadValue = ctx->getIRBuilder()->CreateLoad(lhsValue);
        llvm::Value *subValue = ctx->getIRBuilder()->CreateSub(loadValue, llvm::ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), 1));
        ctx->getIRBuilder()->CreateStore(subValue, lhsValue);
        
        return true;
    }

    bool CodeGeneration::generateCodeForDivideExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        int64_t constantVal = 0;
        if (ctx->getOptimisationsOn() && Utilities::constantFoldedValueFromExpressionNode(node, &constantVal, walker, ctx)) {
            *outValue = Utilities::constantNumberValue(constantVal);
            return true;
        }
        
        return generateCodeForBinaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateSDiv, "divtmp", walker, ctx);
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
        
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        ASTNode paramsNode = NULL;
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            paramsNode = nodeI1;
        ASTNode returnNode = Utilities::getChildNodeAtIndex(node, paramsNode? 2 : 1);
        
        bool isNested = (ctx->getCurrentFunctionProcedureEntity() != NULL);
        
        FunctionEntity *functionEntity = new FunctionEntity(identifier,
                                                            Utilities::getNodeLineNumber(identifierNode),
                                                            std::vector<ParameterEntity*>(),
                                                            Utilities::getTypeFromTypeString(Utilities::getNodeText(returnNode)));
        functionEntity->setIsNestedFunction(isNested);
        addInfoForNestedFunctionOrProcedureToEntity(functionEntity, ctx);
        

        return generateCodeForFunctionProcedureNode(identifierNode,
                                                    paramsNode,
                                                    Utilities::getChildNodeAtIndex(node, paramsNode? 3 : 2),
                                                    functionEntity,
                                                    outValue,
                                                    walker,
                                                    ctx);
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
          /*  ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope())); */
        }
        
        if (!variableEntity->getType().isArray())
            value = ctx->getIRBuilder()->CreateLoad(value);

        if (outValue)
            *outValue = value;
        
        return true;
    }

    bool CodeGeneration::generateCodeForIfStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Function *function = ctx->getCurrentFunctionProcedureEntity()->getLLVMFunction();
        llvm::IRBuilder<> *builder = ctx->getIRBuilder();
        llvm::BasicBlock *afterBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "after");
        llvm::BasicBlock *lastBlock = NULL;
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(node); ++i) {
            ASTNode node1 = Utilities::getChildNodeAtIndex(node, i);
            if (Utilities::getNodeType(node1) == EXPRESSION) {
                llvm::Value *condValue = NULL;

                bool lastIf = i + 1 == Utilities::getNumberOfChildNodes(node) - 1;
                
                walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, i), &condValue, ctx);
                
                llvm::BasicBlock *headerBlock = builder->GetInsertBlock();
                llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then", function);
                llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "else");
                
                // Generate code for the block
                builder->SetInsertPoint(thenBlock);
                walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, i+1), NULL, ctx);
                
                if (!hasReturnInstruction(thenBlock)) {
                    builder->CreateBr(afterBlock);
                }
            
                builder->SetInsertPoint(headerBlock);
                
                if (!lastIf) {
                    builder->CreateCondBr(condValue, thenBlock, elseBlock);
                }
                else
                    builder->CreateCondBr(condValue, thenBlock, afterBlock);
                
                if (!lastIf) {
                    // Insert the 'else' block
                    function->getBasicBlockList().push_back(elseBlock);
                    builder->SetInsertPoint(elseBlock);
                    
                    // We will want to check whether this block contains a return statement if it is the last block.
                    lastBlock = elseBlock;
                } else
                    lastBlock = thenBlock;
                
                // We've looked at the next node too
                ++i;
            }
            else {
                walker->generateCodeForNode(node1, NULL, ctx);
                if (!hasReturnInstruction(lastBlock)) {
                    builder->CreateBr(afterBlock);
                }
            }
        }
        
        function->getBasicBlockList().push_back(afterBlock);
        builder->SetInsertPoint(afterBlock);
    
        return true;
    }

    bool CodeGeneration::generateCodeForIncrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *lhsValue = getLLVMValueFromLValueNode(Utilities::getChildNodeAtIndex(node, 0), walker, ctx);

        llvm::Value *loadedValue = ctx->getIRBuilder()->CreateLoad(lhsValue);
        llvm::Value *incrementedValue = ctx->getIRBuilder()->CreateAdd(loadedValue, llvm::ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), 1));
        ctx->getIRBuilder()->CreateStore(incrementedValue, lhsValue);
        
        return true;
    }

    bool CodeGeneration::generateCodeForInputStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        
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
        std::string inputPrompt = std::string(1, (char)0x0a) + "===> ";
        
        llvm::Value *promptStringValue = ctx->getStringTable()->cachedStringConstant(inputPrompt);
        if (!promptStringValue) {
            promptStringValue = ctx->getIRBuilder()->CreateGlobalStringPtr(inputPrompt);
            ctx->getStringTable()->cacheStringConstant(inputPrompt, promptStringValue);
        }
        
        ctx->getIRBuilder()->CreateCall(Utilities::getPrintfFunction(ctx->getModule()),
                                        promptStringValue);
        
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
    
    bool CodeGeneration::generateCodeForBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(leftParamValue, rightParamValue, twine, false);
        
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

        llvm::BasicBlock *leftAndBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "and_left", function);
        builder->CreateBr(leftAndBlock);
        builder->SetInsertPoint(leftAndBlock);
        
        llvm::Value *leftExpressionValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &leftExpressionValue, ctx);
        llvm::BasicBlock *leftExpressionExitBlock = builder->GetInsertBlock();
        
        llvm::BasicBlock *rightAndBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "and_right");
        llvm::BasicBlock *afterAndBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "and_after");
        
        // Create the conditional branch instruction to implement left-strictness.
        builder->CreateCondBr(leftExpressionValue, rightAndBlock, afterAndBlock);
        
        // Generate code for the right expression to the && operation
        llvm::Value *rightExpressionValue = NULL;
        builder->SetInsertPoint(rightAndBlock);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &rightExpressionValue, ctx);
        llvm::BasicBlock *rightExpressionExitBlock = builder->GetInsertBlock();
        
        // LLVM IR requires that each block is terminated by a branch or return instruction
        builder->CreateBr(afterAndBlock);
        
        // Add the new blocks to the function
        function->getBasicBlockList().push_back(rightAndBlock);
        function->getBasicBlockList().push_back(afterAndBlock);
        
        builder->SetInsertPoint(afterAndBlock);
        
        llvm::PHINode *phiNode = ctx->getIRBuilder()->CreatePHI(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeBoolean)), 2,
                                                                "andtmp");
        
        phiNode->addIncoming(leftExpressionValue, leftExpressionExitBlock);
        phiNode->addIncoming(rightExpressionValue, rightExpressionExitBlock);
        
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
        llvm::BasicBlock *leftExpressionExitBlock = builder->GetInsertBlock();
        
        // Create the conditional branch which means we skip the right expression evaluation if the left expression evaluates to true.
        builder->CreateCondBr(leftExpressionValue, afterOrBlock, rightOrBlock);
        
        // Generate code for the right expression
        llvm::Value *rightExpressionValue = NULL;
        builder->SetInsertPoint(rightOrBlock);
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 1), &rightExpressionValue, ctx);
        llvm::BasicBlock *rightExpressionExitBlock = builder->GetInsertBlock();
        
        // LLVM IR requires that each block is terminated by a branch or return instruction
        builder->CreateBr(afterOrBlock);
        
        // Add the blocks to the function
        function->getBasicBlockList().push_back(rightOrBlock);
        function->getBasicBlockList().push_back(afterOrBlock);
        
        builder->SetInsertPoint(afterOrBlock);
        
        llvm::PHINode *phiNode = ctx->getIRBuilder()->CreatePHI(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeBoolean)), 2,
                                                                "ortmp");
        
        phiNode->addIncoming(leftExpressionValue, leftExpressionExitBlock);
        phiNode->addIncoming(rightExpressionValue, rightExpressionExitBlock);
        
        if (outValue)
            *outValue = phiNode;
        
        return true;
    }
    
    bool CodeGeneration::generateCodeForMinusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) == 1)
            return generateCodeForUnaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateNeg, "negtmp", walker, ctx);
        
        // Attempt to constant fold
        int64_t constantVal = 0;
        if (ctx->getOptimisationsOn() && Utilities::constantFoldedValueFromExpressionNode(node, &constantVal, walker, ctx)) {
            *outValue = Utilities::constantNumberValue(constantVal);
            return true;
        }
        
        return generateCodeForBinaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateSub, "subtmp", walker, ctx);
    }
    
    bool CodeGeneration::generateCodeForModuloExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        // Attempt to constant fold
        int64_t constantVal = 0;
        if (ctx->getOptimisationsOn() && Utilities::constantFoldedValueFromExpressionNode(node, &constantVal, walker, ctx)) {
            *outValue = Utilities::constantNumberValue(constantVal);
            return true;
        }
        
        return generateCodeForBinaryOperatorNode(node, outValue,
                                       &llvm::IRBuilder<>::CreateSRem,
                                       "modtmp",
                                       walker,
                                       ctx);
    }
    
    bool CodeGeneration::generateCodeForMultiplyExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        // Attempt to constant fold
        int64_t constantVal = 0;
        if (ctx->getOptimisationsOn() && Utilities::constantFoldedValueFromExpressionNode(node, &constantVal, walker, ctx)) {
            *outValue = Utilities::constantNumberValue(constantVal);
            return true;
        }
        
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
                                       &llvm::IRBuilder<>::CreateICmpNE,
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
            *outValue = llvm::ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), val);
        
        return true;
    }

    bool CodeGeneration::generateCodeForNumberTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForParamsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
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
        
        // Attempt to constant fold
        int64_t constantVal = 0;
        if (ctx->getOptimisationsOn() && Utilities::constantFoldedValueFromExpressionNode(node, &constantVal, walker, ctx)) {
            *outValue = Utilities::constantNumberValue(constantVal);
            return true;
        }
        
        return generateCodeForBinaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateAdd, "addtmp", walker, ctx);
    }

    bool CodeGeneration::generateCodeForProcedureDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        ASTNode paramsNode = NULL;
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            paramsNode = nodeI1;
        
        bool isNested = (ctx->getCurrentFunctionProcedureEntity() != NULL);
        ProcedureEntity *procedureEntity = new ProcedureEntity(identifier,
                                                               Utilities::getNodeLineNumber(identifierNode),
                                                               std::vector<ParameterEntity*>());
        procedureEntity->setIsNestedFunction(isNested);

        return generateCodeForFunctionProcedureNode(identifierNode,
                                                    paramsNode,
                                                    Utilities::getChildNodeAtIndex(node, paramsNode? 2 : 1),
                                                    procedureEntity,
                                                    outValue,
                                                    walker,
                                                    ctx);

    }

    bool CodeGeneration::generateCodeForPrintStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {        
        llvm::Function *printfFunction = Utilities::getPrintfFunction(ctx->getModule());
        
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
        
        FunctionProcedureEntity *funcProcEntity = dynamic_cast<FunctionProcedureEntity*>(entity);
        
        std::vector<llvm::Value*> arguments;
        llvm::Value *structAlloc = NULL;
        
        if (funcProcEntity->getIsNestedFunction()) {
            llvm::StructType *structType = funcProcEntity->getContextStructType();
            
            structAlloc = ctx->getIRBuilder()->CreateAlloca(structType);
            
            arguments.push_back(structAlloc);
        }
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(identifierNode); ++i) {
            llvm::Value *value = NULL;
            walker->generateCodeForNode(Utilities::getChildNodeAtIndex(identifierNode, i), &value, ctx);
            arguments.push_back(value);
        }
        
        llvm::ArrayRef<llvm::Value*> llvmArguments = llvm::makeArrayRef(arguments);

        handleNestedFunctionBeforeInvocation(funcProcEntity, structAlloc, ctx);

        if (Utilities::getTypeOfEntity(funcProcEntity) == MAliceEntityTypeFunction) {
            llvm::Value * v = ctx->getIRBuilder()->CreateCall(funcProcEntity->getLLVMFunction(), llvmArguments, "calltmp");
            if (outValue)
                *outValue = v;
        } else {
            ctx->getIRBuilder()->CreateCall(funcProcEntity->getLLVMFunction(), llvmArguments);
        }
        
        handleNestedFunctionAfterInvocation(funcProcEntity, structAlloc, ctx);

        return true;
    }

    bool CodeGeneration::generateCodeForProgramNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::BasicBlock *progRootBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "progRoot");
        ctx->getIRBuilder()->SetInsertPoint(progRootBlock);

        return walker->generateCodeForChildren(node, NULL, ctx);
    }

    bool CodeGeneration::generateCodeForReturnStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *returnValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &returnValue, ctx);


        storeElementsIntoNestedFunctionStruct(ctx);
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
        strVal = Utilities::stringWithASCIIControlCodes(strVal.c_str());
        
        llvm::Value *value = ctx->getStringTable()->cachedStringConstant(strVal);
        
        if (!value) {
            // Global variables have to be created slightly differently...
            if (!ctx->getCurrentFunctionProcedureEntity()) {
                llvm::Constant *stringConstant = llvm::ConstantArray::get(llvm::getGlobalContext(), strVal);
                llvm::GlobalVariable *stringGlobal = new llvm::GlobalVariable(*(ctx->getModule()),
                                                                              stringConstant->getType(),
                                                                              true,
                                                                              llvm::GlobalVariable::PrivateLinkage,
                                                                              stringConstant,
                                                                              "str");
                
                std::vector<llvm::Constant*> constants;
                llvm::ConstantInt *zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), 0);
                constants.push_back(zeroIndex);
                constants.push_back(zeroIndex);

                value = llvm::ConstantExpr::getInBoundsGetElementPtr(stringGlobal, constants);
            }
            else
                value = ctx->getIRBuilder()->CreateGlobalStringPtr(Utilities::stringWithASCIIControlCodes(strVal.c_str()));
            
            ctx->getStringTable()->cacheStringConstant(strVal, value);
        }
        
        if (outValue)
            *outValue = value;
        
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
        
        llvm::GlobalVariable *value = new llvm::GlobalVariable(*(ctx->getModule()),
                                                               type,
                                                               false,
                                                               llvm::GlobalValue::InternalLinkage,
                                                               initialiserValue,
                                                               llvm::Twine(identifier.c_str()));
        
        variable->setLLVMValue(value);
        ctx->addEntityInScope(identifier, variable);
        
        // Add debug info
        if (ctx->getDGBuilder())
        {
            ctx->getDGBuilder()->createGlobalVariable(identifier,
                                                      *ctx->getDIFile(),
                                                      Utilities::getNodeLineNumber(node),
                                                      Utilities::getDILLVMType(variable->getType(), type, ctx),
                                                      false,
                                                      value);
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


        llvm::Type *type = Utilities::getLLVMTypeFromType(variable->getType());
        llvm::Value *value = ctx->getIRBuilder()->CreateAlloca(type, NULL, identifier.c_str());
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
                                                    Utilities::getDILLVMType(variable->getType(), type, ctx),
                                                    true);
            llvm::Instruction *Call = ctx->getDGBuilder()->insertDeclare(value, debugVar, ctx->getIRBuilder()->GetInsertBlock());
            Call->setDebugLoc(llvm::DebugLoc::get(Utilities::getNodeLineNumber(identifierNode),Utilities::getNodeColumnIndex(identifierNode), ctx->getCurrentDBScope())); 
          /*  ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope())); */

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
        
        llvm::BasicBlock *loopHeader = llvm::BasicBlock::Create(llvm::getGlobalContext(), "loopheader", function);
        builder->CreateBr(loopHeader);
        
        llvm::BasicBlock *loopBody = llvm::BasicBlock::Create(llvm::getGlobalContext(), "loopbody");
        llvm::BasicBlock *afterLoopBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "afterloop");
        
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
        auto it = function->arg_begin();
        if (funcProcEntity->getIsNestedFunction())
            it++;
        
        for (; it != function->arg_end(); ++it) {
            ParameterEntity *parameterEntity = funcProcEntity->getParameterListTypes().at(i);
            
            llvm::Value *alloca = builder->CreateAlloca(Utilities::getLLVMTypeFromType(parameterEntity->getType()));
            builder->CreateStore(it, alloca);
            
            llvm::Value *paramValue = alloca;
            if (parameterEntity->getByReference())
                paramValue = builder->CreateLoad(alloca);
            
            parameterEntity->setLLVMValue(paramValue);

            if (ctx->getDGBuilder())
            {
                llvm::DIVariable debugVar = ctx->getDGBuilder()->createLocalVariable(llvm::dwarf::DW_TAG_auto_variable,
                                        llvm::DIDescriptor(ctx->getCurrentDBScope()),
                                        parameterEntity->getIdentifier(),
                                        *ctx->getDIFile(),
                                        NULL,
                                        Utilities::getDILLVMType(parameterEntity->getType(), NULL, ctx),
                                        true);
                ctx->getDGBuilder()->insertDeclare(alloca, debugVar, ctx->getIRBuilder()->GetInsertBlock());
            }

            
            ++i;
        }
    }
    
    bool CodeGeneration::hasReturnInstruction(llvm::BasicBlock *block)
    {
        for (auto it = block->begin(); it != block->end(); ++it) {
            llvm::Instruction *instruction = it;
            
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
        llvm::StructType *structType = funcProcEntity->getContextStructType();
        
        if (structType) {
            parameterTypes.push_back(llvm::PointerType::get(structType, 0));
        }
        
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
        
        llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, parameterTypes, false);
        std::string identifier = funcProcEntity->getIdentifier();
        std::string LLVMIdentifier;
        if (isEntryPoint)
            LLVMIdentifier = ctx->getIdentifierDispenser()->identifierForEntryPointFunction();
        else
            LLVMIdentifier = ctx->getIdentifierDispenser()->identifierForFunctionWithName(identifier);
        
        llvm::Function *function = llvm::Function::Create(functionType,
                                                          llvm::Function::ExternalLinkage,
                                                          LLVMIdentifier,
                                                          ctx->getModule());
        
        auto argIt = function->arg_begin();
        if (structType) {
            llvm::Value *arg = argIt;
            arg->setName("s");
            ++argIt;
        }
        
        // Set the arg names in the LLVM function
        unsigned int i = 0;
        for (; argIt != function->arg_end(); ++argIt) {
            ParameterEntity *entity = parameterEntities.at(i);
            
            llvm::Value *arg = argIt;
            arg->setName(entity->getIdentifier());
            
            ++i;
        }
        
        return function;
    }
    
    void CodeGeneration::extractElementsFromNestedFunctionStruct(CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = ctx->getCurrentFunctionProcedureEntity();
        if (!funcProcEntity->getIsNestedFunction())
            return;
        
        llvm::StructType *structType = funcProcEntity->getContextStructType();
        std::vector<std::string> capturedIdentifiers = funcProcEntity->getCapturedVariables();
        
        llvm::Function *function = funcProcEntity->getLLVMFunction();
        llvm::Value *firstArg = function->arg_begin();
        
        unsigned int i = 0;
        for (auto it = capturedIdentifiers.begin(); it != capturedIdentifiers.end(); ++it) {
            std::string identifier = *it;
            
            llvm::Type *type = structType->getElementType(i);
            llvm::Value *localVariable = ctx->getIRBuilder()->CreateAlloca(type, 0, identifier);
            llvm::Value *structValue = ctx->getIRBuilder()->CreateGEP(firstArg, Utilities::llvmStructElementGEPIndexes(i));
            llvm::Value *loadValue = ctx->getIRBuilder()->CreateLoad(structValue);
            ctx->getIRBuilder()->CreateStore(loadValue, localVariable);
            
            Entity *outerScopeEntity = NULL;
            ctx->isSymbolInScope(identifier, &outerScopeEntity);
            
            VariableEntity *outerScopeVariableEntity = dynamic_cast<VariableEntity*>(outerScopeEntity);
            if (outerScopeVariableEntity) {
                VariableEntity *variableEntity = new VariableEntity(identifier,
                                                                    0, // We're beyond semantic analysis, so we don't care what line number it's on.
                                                                    outerScopeVariableEntity->getType());
                
                llvm::Value *value = localVariable;
                if (variableEntity->getType().isArray())
                    value = ctx->getIRBuilder()->CreateLoad(value);
                
                variableEntity->setLLVMValue(value);
                ctx->addEntityInScope(identifier, variableEntity);
                if (ctx->getDGBuilder())
                {
                    llvm::DIVariable debugVar = ctx->getDGBuilder()->createLocalVariable(llvm::dwarf::DW_TAG_auto_variable,
                                            llvm::DIDescriptor(ctx->getCurrentDBScope()),
                                            identifier,
                                            *ctx->getDIFile(),
                                            NULL,
                                            Utilities::getDILLVMType(variableEntity->getType(), NULL, ctx),
                                            true);
                    ctx->getDGBuilder()->insertDeclare(localVariable, debugVar, ctx->getIRBuilder()->GetInsertBlock());
                }
            }
            
            ++i;
        }
    }
    
    void CodeGeneration::storeElementsIntoNestedFunctionStruct(CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = ctx->getCurrentFunctionProcedureEntity();
        if (!funcProcEntity->getIsNestedFunction())
            return;
        
        std::vector<std::string> capturedIdentifiers = funcProcEntity->getCapturedVariables();
        
        llvm::Function *function = funcProcEntity->getLLVMFunction();
        llvm::Value *firstArg = function->arg_begin();
        
        unsigned int i = 0;
        for (auto it = capturedIdentifiers.begin(); it != capturedIdentifiers.end(); ++it) {
            Entity *entity = NULL;
            ctx->isSymbolInScope(*it, &entity);
            
            VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
            
            // Arrays are by-reference so we don't need to put their values back into the struct.
            if (variableEntity && !variableEntity->getType().isArray()) {
                llvm::Value *structValue = ctx->getIRBuilder()->CreateGEP(firstArg, Utilities::llvmStructElementGEPIndexes(i));
                ctx->getIRBuilder()->CreateStore(ctx->getIRBuilder()->CreateLoad(variableEntity->getLLVMValue()), structValue);
            }
            
            ++i;
        }
    }
    
    void CodeGeneration::handleNestedFunctionBeforeInvocation(FunctionProcedureEntity *funcProcEntity, llvm::Value *structAlloc, CompilerContext *ctx)
    {
        if (!funcProcEntity->getIsNestedFunction())
            return;
        
        unsigned int i = 0;
        std::vector<std::string> capturedIdentifiers = funcProcEntity->getCapturedVariables();
        
        for (auto it = capturedIdentifiers.begin(); it != capturedIdentifiers.end(); ++it) {
            Entity *entity = NULL;
            ctx->isSymbolInScope(*it, &entity);
            
            VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
            if (variableEntity) {
                llvm::Value *structValue = ctx->getIRBuilder()->CreateInBoundsGEP(structAlloc, Utilities::llvmStructElementGEPIndexes(i));
                
                Type type = variableEntity->getType();
                llvm::Value *value = variableEntity->getLLVMValue();
                if (!type.isArray())
                    value = ctx->getIRBuilder()->CreateLoad(value);
                
                ctx->getIRBuilder()->CreateStore(value, structValue);
            }
            
            ++i;
        }
    }
    
    void CodeGeneration::handleNestedFunctionAfterInvocation(FunctionProcedureEntity *funcProcEntity, llvm::Value *structAlloc, CompilerContext *ctx)
    {
        if (!funcProcEntity->getIsNestedFunction())
            return;
        
        unsigned int i = 0;
        std::vector<std::string> capturedIdentifiers = funcProcEntity->getCapturedVariables();
        
        for (auto it = capturedIdentifiers.begin(); it != capturedIdentifiers.end(); ++it) {
            Entity *entity = NULL;
            ctx->isSymbolInScope(*it, &entity);
            
            VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
            if (variableEntity && !variableEntity->getType().isArray()) {
                llvm::Value *structValue = ctx->getIRBuilder()->CreateInBoundsGEP(structAlloc, Utilities::llvmStructElementGEPIndexes(i));
                ctx->getIRBuilder()->CreateStore(ctx->getIRBuilder()->CreateLoad(structValue), variableEntity->getLLVMValue());
            }
            
            ++i;
        }
    }
    
    void CodeGeneration::addInfoForNestedFunctionOrProcedureToEntity(FunctionProcedureEntity *funcProcEntity, CompilerContext *ctx)
    {
        if (!funcProcEntity->getIsNestedFunction())
            return;
        
        std::vector<VariableEntity*> variableEntities = ctx->variableEntitiesInCurrentScope();
        std::vector<llvm::Type*> structTypes;
        std::vector<std::string> capturedVariables;
        
        for (auto it = variableEntities.begin(); it != variableEntities.end(); ++it) {
            VariableEntity *variableEntity = *it;
            
            structTypes.push_back(Utilities::getLLVMTypeFromType(variableEntity->getType()));
            capturedVariables.push_back(variableEntity->getIdentifier());
        }
        
        llvm::StructType *contextStructType = llvm::StructType::create(llvm::getGlobalContext(), structTypes, "struct_type." + funcProcEntity->getIdentifier());
        funcProcEntity->setContextStructType(contextStructType);
        funcProcEntity->setCapturedVariables(capturedVariables);
    }
    
    bool CodeGeneration::generateCodeForFunctionProcedureNode( ASTNode identifierNode, ASTNode paramsNode, ASTNode bodyNode, FunctionProcedureEntity *funcProcEntity, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::string funcName = funcProcEntity->getIdentifier();
        bool isEntryProcedure = !funcProcEntity->getIsNestedFunction() && funcName == "hatta";
        addInfoForNestedFunctionOrProcedureToEntity(funcProcEntity, ctx);
        
        // Enter the scope for code generation of the function

        ctx->enterFunctionProcedureScope(funcProcEntity);
        
        if (paramsNode) {
            if (!walker->generateCodeForNode(paramsNode, NULL, ctx))
                return false;
        }
        
        llvm::Function *function = createFunctionForEntity(funcProcEntity, ctx, isEntryProcedure);
        funcProcEntity->setLLVMFunction(function);
        
        if (ctx->getDGBuilder())
        {
            unsigned lineNo = Utilities::getNodeLineNumber(identifierNode);
            
            llvm::DIFile dbFile = *ctx->getDIFile();
            llvm::DIArray paramArray = llvm::DIArray(0); // TODO: Populate parameters..
            llvm::DIType subType = ctx->getDGBuilder()->createSubroutineType(dbFile, paramArray);
            llvm::DISubprogram SP = ctx->getDGBuilder()->createFunction(dbFile, funcName, isEntryProcedure?"main":funcName, dbFile,
                                lineNo, subType,
                                true, true, false,
                                function, 0, 0);

            llvm::MDNode * SPN = SP;

            ctx->enterDebugScope(SPN);

        }

        bool result = walker->generateCodeForNode(bodyNode, NULL, ctx);
        
        if (isEntryProcedure) {
            // Insert an exit() call
            llvm::Function *exitFunction = Utilities::getExitFunction(ctx->getModule());
            ctx->getIRBuilder()->CreateCall(exitFunction, llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()),
                                                                                 0));
        }
        
        if (dynamic_cast<ProcedureEntity*>(funcProcEntity)) {
            storeElementsIntoNestedFunctionStruct(ctx);
            
            // LLVM requires all functions to return a value
            ctx->getIRBuilder()->CreateRetVoid();
        }
        else {
            FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(funcProcEntity);
            
            if (!hasReturnInstruction(ctx->getIRBuilder()->GetInsertBlock())) {
                storeElementsIntoNestedFunctionStruct(ctx);
                ctx->getIRBuilder()->CreateRet(Utilities::llvmDefaultValueForType(functionEntity->getReturnType()));
            }
        }
        
        // Exit the scope again
        ctx->exitFunctionProcedureScope();
        if (ctx->getDGBuilder())
            ctx->exitDebugScope();

        if (!result) {
            // Remove the function from the module it's a part of.
            function->removeFromParent();
            return false;
        }
        
        if (outValue)
            *outValue = function;

        return true;
    }
};
