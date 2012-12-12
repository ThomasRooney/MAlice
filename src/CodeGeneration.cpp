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
#include "llvm/Value.h"
#include "llvm/Support/Dwarf.h"
#include "llvm/Analysis/DebugInfo.h"

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
        llvm::Value *lvalueValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &lvalueValue, ctx);
        
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
        ctx->enterScope();
        ctx->enterDebugScope(node);
        
        bool result = walker->generateCodeForChildren(node, NULL, ctx);
        
        ctx->exitScope();
        ctx->exitDebugScope(node);
        
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
        llvm::Value *lhsValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &lhsValue, ctx);

        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        
        ctx->getIRBuilder()->CreateSub(lhsValue, ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), 1));
        
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
        Type returnType = Utilities::getTypeFromTypeString(Utilities::getNodeText(returnNode));
        
        ASTNode bodyNode = Utilities::getChildNodeAtIndex(node, hasParams?3:2);
        
        FunctionEntity *functionEntity = new FunctionEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::vector<ParameterEntity*>(), returnType);
        ctx->addEntityInScope(identifier, functionEntity);
        ctx->pushFunctionProcedureEntity(functionEntity);
        
        // Populate the function arguments.
        if (hasParams) {
            if (!walker->generateCodeForNode(nodeI1, NULL, ctx))
                return false;
        }
        
        // Create the llvm::Function and insert it into the module
        std::vector<ParameterEntity*> parameterEntities = functionEntity->getParameterListTypes();
        std::vector<llvm::Type*> parameterTypes;
        for (auto it = parameterEntities.begin(); it != parameterEntities.end(); ++it) {
            ParameterEntity *entity = *it;
            llvm::Type *llvmType = Utilities::getLLVMTypeFromType(entity->getType());
            
            parameterTypes.push_back(llvmType);
        }
        
        FunctionType *functionType = FunctionType::get(Utilities::getLLVMTypeFromType(functionEntity->getReturnType()),
                                                       parameterTypes,
                                                       false);
        
        Function *function = Function::Create(functionType,
                                              Function::InternalLinkage,
                                              identifier.c_str(),
                                              ctx->getModule());
        
        functionEntity->setLLVMFunction(function);
        
        BasicBlock *bodyBlock = BasicBlock::Create(getGlobalContext(), "entry", function);
        ctx->getIRBuilder()->SetInsertPoint(bodyBlock);
    
        // Walk through the children
        bool result = walker->generateCodeForNode(bodyNode, NULL, ctx);
        
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
        
        if (Utilities::getTypeOfEntity(entity) == MAliceEntityTypeGlobalVariable) {
            if (outValue)
                *outValue = value;
            
            return true;
        }

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
        Function *function = NULL;
        BasicBlock *insertBlock = ctx->getIRBuilder()->GetInsertBlock();
        if (insertBlock)
            function = insertBlock->getParent();
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(node); ++i) {
            ASTNode node1 = Utilities::getChildNodeAtIndex(node, i);
            if (Utilities::getNodeType(node1) == EXPRESSION) {
                llvm::Value *condValue = NULL;
                walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &condValue, ctx);
                
                ASTNode node2 = Utilities::getChildNodeAtIndex(node, i + 1);
                // We have looked at another node.
                i++;
                
                BasicBlock *thenBlock = BasicBlock::Create(getGlobalContext(), "then", function);
                BasicBlock *elseBlock = BasicBlock::Create(getGlobalContext(), "else", function);
                
                // Change debug line number
                if (ctx->getDGBuilder())
                {
                    ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                                        Utilities::getNodeColumnIndex(node),
                                                                                        ctx->getCurrentDBScope()));
                }                                       


                ctx->getIRBuilder()->CreateCondBr(condValue, thenBlock, elseBlock);
                
                ctx->getIRBuilder()->SetInsertPoint(thenBlock);
                walker->generateCodeForNode(node2, NULL, ctx);
                
                ctx->getIRBuilder()->SetInsertPoint(elseBlock);
            }
            else {
                walker->generateCodeForNode(node1, NULL, ctx);
                
                BasicBlock *afterBlock = BasicBlock::Create(getGlobalContext(), "after", function);
                ctx->getIRBuilder()->SetInsertPoint(afterBlock);
            }
        }
        
        return true;
    }

    bool CodeGeneration::generateCodeForIncrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *lhsValue = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &lhsValue, ctx);
        
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


        std::vector<llvm::Type*> parameterTypes;
        parameterTypes.push_back(llvm::Type::getInt8PtrTy(getGlobalContext()));
        
        FunctionType *scanfFunctionType = FunctionType::get(llvm::Type::getInt32Ty(getGlobalContext()),
                                                            parameterTypes,
                                                            true);
        
        Function *scanfFunction = cast<Function>(ctx->getModule()->getOrInsertFunction("scanf", scanfFunctionType));
        
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
        
        llvm::Value *formatStringValue = ctx->ioFormatStringForExpressionType(type);
        
        // Create the scanf() call.
        ctx->getIRBuilder()->CreateCall2(scanfFunction, formatStringValue, value);
        
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
        return walker->generateCodeForChildren(node, NULL, ctx);
    }
    
    bool CodeGeneration::generateCodeForLogicalNotExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return generateCodeForUnaryOperatorNode(node,
                                      outValue,
                                      &llvm::IRBuilder<>::CreateNot,
                                      "nottmp",
                                      walker,
                                      ctx);
    }

    bool CodeGeneration::generateCodeForLogicalOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
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
            llvm::Value *parameterValue = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromType(entity->getType()), 0, entity->getIdentifier().c_str());
            entity->setLLVMValue(parameterValue);
            ctx->addEntityInScope(entity->getIdentifier(), entity);
        }
        
        entity->setParameterListTypes(parameterList);
        
        return true;
    }

    bool CodeGeneration::generateCodeForPlusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) == 1) {
            // TODO: Implement absolute value transform.
            
            return true;
        }
        
        return generateCodeForBinaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateSub, "addtmp", walker, ctx);
    }

    bool CodeGeneration::generateCodeForProcedureDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ProcedureEntity *procedureEntity = new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::vector<ParameterEntity*>());
        std::string LLVMIdentifier;
        if (!ctx->getCurrentFunctionProcedureEntity() && identifier == "hatta")
            LLVMIdentifier = "main";
        else
            LLVMIdentifier = ctx->getIdentifierDispenser()->identifierForFunctionWithName(identifier);
        
        ctx->addEntityInScope(identifier, procedureEntity);
        ctx->pushFunctionProcedureEntity(procedureEntity);
        
        bool hasParams = false;
        
        // get node index 1, if its a parameter node, get params...
        ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
        if (Utilities::getNodeType(nodeI1) == PARAMS)
            hasParams = true;
        ASTNode bodyNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
        
        if (hasParams) {
            if (!walker->generateCodeForNode(nodeI1, NULL, ctx))
                return false;
        }
        
        // Create the llvm::Function for the procedure and add it to the llvm::Module.
        std::vector<llvm::Type*> parameterTypes;
        std::vector<ParameterEntity*> parameterEntities = procedureEntity->getParameterListTypes();
        
        for (auto it = parameterEntities.begin(); it != parameterEntities.end(); ++it) {
            ParameterEntity *entity = *it;
            parameterTypes.push_back(Utilities::getLLVMTypeFromType(entity->getType()));
        }
        
        ArrayRef<llvm::Type*> parameterArrayRefs = makeArrayRef(parameterTypes);
        FunctionType *procedureType = FunctionType::get(llvm::Type::getVoidTy(getGlobalContext()),
                                                        parameterArrayRefs,
                                                        false);
        
        Function *procedure = Function::Create(procedureType,
                                               Function::ExternalLinkage,
                                               LLVMIdentifier.c_str(),
                                               ctx->getModule());
        
        procedureEntity->setLLVMFunction(procedure);
        
        BasicBlock *block = BasicBlock::Create(getGlobalContext(), "entry", procedure);
        ctx->getIRBuilder()->SetInsertPoint(block);
        
        bool result = walker->generateCodeForNode(bodyNode, NULL, ctx);
        if (!result) {
            // Remove the procedure from the Module it's a part of.
            procedure->removeFromParent();
            return false;
        }
        
        ctx->popFunctionProcedureEntity();
        
        // Create the return value for the function, which will exit the scope for the function.
        ctx->getIRBuilder()->CreateRetVoid();
        
        if (outValue)
            *outValue = procedure;
        
        return true;
    }

    bool CodeGeneration::generateCodeForPrintStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::vector<llvm::Type*> parameterTypes;
        parameterTypes.push_back(llvm::Type::getInt8PtrTy(getGlobalContext()));
        
        FunctionType *printfFunctionType = FunctionType::get(llvm::Type::getInt32Ty(getGlobalContext()),
                                                             parameterTypes,
                                                             true);
        
        Function *printfFunction = cast<Function>(ctx->getModule()->getOrInsertFunction("printf", printfFunctionType));
        
        Type type;
        Utilities::getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0),
                                             &type,
                                             false,
                                             walker,
                                             ctx,
                                             NULL);
        
        llvm::Value *printVal = NULL;
        walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &printVal, ctx);
        
        llvm::Value *formatStringValue = ctx->ioFormatStringForExpressionType(type);
        
        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

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
            // Load the value into an argument
            llvm::Value *arg = ctx->getIRBuilder()->CreateLoad(value);
            arguments.push_back(arg);
        }
        
        FunctionProcedureEntity *funcProcEntity = dynamic_cast<FunctionProcedureEntity*>(entity);
        
        llvm::ArrayRef<llvm::Value*> a(&arguments[0], arguments.size());

        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        llvm::Value * v = ctx->getIRBuilder()->CreateCall(funcProcEntity->getLLVMFunction(), a, "calltmp");
        if (outValue)
            *outValue = v;



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
            *outValue = ctx->getIRBuilder()->CreateGlobalStringPtr(strVal.c_str(), "string");
        
        return true;
    }

    bool CodeGeneration::generateCodeForTildeExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->generateCodeForChildren(node, NULL, ctx);
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
        llvm::GlobalVariable *value = new GlobalVariable(*(ctx->getModule()),
                                                         llvm::PointerType::get(Utilities::getLLVMTypeFromType(variable->getType()), 0),
                                                         false,
                                                         GlobalValue::InternalLinkage,
                                                         NULL,
                                                         Twine(identifier.c_str()));
        
        ConstantPointerNull* constNullPtr = ConstantPointerNull::get(llvm::Type::getInt64PtrTy(getGlobalContext()));
        value->setInitializer(constNullPtr);
        
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
            *outValue = value;
        
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


        llvm::Value *value = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromType(variable->getType()),
                                                               0,
                                                               identifier.c_str());
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
        return true;
        
        BasicBlock *currentBlock = ctx->getIRBuilder()->GetInsertBlock();
        Function *parentFunction = currentBlock->getParent();
        
        llvm::Value *conditionValue = NULL;
        if (!walker->generateCodeForNode(Utilities::getChildNodeAtIndex(node, 0), &conditionValue, ctx))
            return false;
        
        BasicBlock *loopHeaderBlock = ctx->getIRBuilder()->GetInsertBlock();
        BasicBlock *loopBody = BasicBlock::Create(getGlobalContext(), "loop", parentFunction);
        ctx->getIRBuilder()->SetInsertPoint(loopBody);
        
        BasicBlock *afterLoopBodyBlock = BasicBlock::Create(getGlobalContext(), "afterloop", parentFunction);
        
        ctx->getIRBuilder()->SetInsertPoint(loopHeaderBlock);

        // Change debug line number
        if (ctx->getDGBuilder())
        {
            ctx->getIRBuilder()->SetCurrentDebugLocation(llvm::DebugLoc::get(Utilities::getNodeLineNumber(node),
                                                                             Utilities::getNodeColumnIndex(node),
                                                                             ctx->getCurrentDBScope()));
        }                                       

        ctx->getIRBuilder()->CreateCondBr(conditionValue, loopBody, afterLoopBodyBlock);

        ctx->getIRBuilder()->SetInsertPoint(afterLoopBodyBlock);

        return true;
    }
    
    llvm::Value *CodeGeneration::getLLVMValueFromLValueNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
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
};