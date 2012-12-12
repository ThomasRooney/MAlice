#include <string>
#include <sstream>
#include <list>
#include <typeinfo>

#include "VisitorCallbacks.h"
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
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ASTNode typeNode = Utilities::getChildNodeAtIndex(identifierNode, 0);
        std::string typeString = Utilities::getNodeText(typeNode);
        
        Type arrayType = Utilities::getTypeFromTypeString(typeString);
        arrayType.setIsArray(true);
        VariableEntity *arrayEntity = new VariableEntity(identifier, Utilities::getNodeLineNumber(node), arrayType);
        ctx->addEntityInScope(identifier, arrayEntity);
        
        llvm::Value *value = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromType(arrayEntity->getType()));
        arrayEntity->setLLVMValue(value);
        
        return true;
    }

    bool visitArraySubscriptNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitAssignmentStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *lvalueValue = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &lvalueValue, ctx);
        
        llvm::Value *assignmentValue = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &assignmentValue, ctx);
        
        if (outValue)
            *outValue = ctx->getIRBuilder()->CreateStore(assignmentValue, lvalueValue);
        
        return true;
    }

    bool visitBitwiseAndExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateAnd,
                                       "andtmp",
                                       walker,
                                       ctx);
    }
    
    bool visitBitwiseOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateOr,
                                       "andtmp",
                                       walker,
                                       ctx);
    }
    
    bool visitBitwiseXorExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateXor,
                                       "xortmp",
                                       walker,
                                       ctx);
    }

    bool visitBodyNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ctx->enterScope();
        
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
        std::string strVal = Utilities::getNodeText(node);
        uint64_t val = strVal[1]; // [0] = "'", [1] = LITERAL, [2] = "'"
        
        if (outValue)
            *outValue = ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeLetter)), val);
        
        return true;
    }

    bool visitDeclarationsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitDecrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *lhsValue = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &lhsValue, ctx);
        
        ctx->getIRBuilder()->CreateSub(lhsValue, ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), 1));
        
        return true;
    }

    bool visitDivideExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitEqualsExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateICmpEQ,
                                       "eqtmp",
                                       walker,
                                       ctx);
    }

    bool visitExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        bool result;
        ctx->beginExpression();
        // Populate the Expression Code Generation
        
        // We are going to have only one child
        ASTNode childNode = Utilities::getChildNodeAtIndex(node, 0);
        result = walker->visitNode(childNode, outValue, ctx);
        ctx->endExpression();
        
        return result;
    }

    bool visitFunctionDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
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
            if (!walker->visitNode(nodeI1, NULL, ctx))
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
        bool result = walker->visitNode(bodyNode, NULL, ctx);
        
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
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateICmpSGT,
                                       "gttmp",
                                       walker,
                                       ctx);
    }
    
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateICmpSGE,
                                       "gtetmp",
                                       walker,
                                       ctx);
    }
    
    bool visitIdentifierNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
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

        if (outValue)
            *outValue = value;
        
        return true;
    }

    bool visitIfStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        Function *function = NULL;
        BasicBlock *insertBlock = ctx->getIRBuilder()->GetInsertBlock();
        if (insertBlock)
            function = insertBlock->getParent();
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(node); ++i) {
            ASTNode node1 = Utilities::getChildNodeAtIndex(node, i);
            if (Utilities::getNodeType(node1) == EXPRESSION) {
                llvm::Value *condValue = NULL;
                walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &condValue, ctx);
                
                ASTNode node2 = Utilities::getChildNodeAtIndex(node, i + 1);
                // We have looked at another node.
                i++;
                
                BasicBlock *thenBlock = BasicBlock::Create(getGlobalContext(), "then", function);
                BasicBlock *elseBlock = BasicBlock::Create(getGlobalContext(), "else", function);
                
                ctx->getIRBuilder()->CreateCondBr(condValue, thenBlock, elseBlock);
                
                ctx->getIRBuilder()->SetInsertPoint(thenBlock);
                walker->visitNode(node2, NULL, ctx);
                
                ctx->getIRBuilder()->SetInsertPoint(elseBlock);
            }
            else {
                walker->visitNode(node1, NULL, ctx);
                
                BasicBlock *afterBlock = BasicBlock::Create(getGlobalContext(), "after", function);
                ctx->getIRBuilder()->SetInsertPoint(afterBlock);
            }
        }
        
        return true;
    }

    bool visitIncrementStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *lhsValue = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &lhsValue, ctx);
        
        ctx->getIRBuilder()->CreateAdd(lhsValue, ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), 1));
        
        return true;
    }

    bool visitInputStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::vector<llvm::Type*> parameterTypes;
        parameterTypes.push_back(llvm::Type::getInt8PtrTy(getGlobalContext()));
        
        FunctionType *scanfFunctionType = FunctionType::get(llvm::Type::getInt32Ty(getGlobalContext()),
                                                            parameterTypes,
                                                            true);
        
        Function *scanfFunction = cast<Function>(ctx->getModule()->getOrInsertFunction("scanf", scanfFunctionType));
        
        llvm::Value *inputVal = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &inputVal, ctx);
        
        Type type;
        Utilities::getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0),
                                             &type,
                                             false,
                                             walker,
                                             ctx,
                                             NULL);
        
        llvm::Value *formatStringValue = ctx->ioFormatStringForExpressionType(type);
        
        // Create the scanf() call.
        ctx->getIRBuilder()->CreateCall2(scanfFunction, formatStringValue, inputVal);
        
        return true;
    }

    bool visitLessThanExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateICmpSLT,
                                       "lttemp",
                                       walker,
                                       ctx);
    }
    
    bool visitLessThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateICmpSLE,
                                       "ltetmp",
                                       walker,
                                       ctx);
    }
    
    bool visitBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(leftParamValue, rightParamValue, twine);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
    }
    
    bool visitBinaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(leftParamValue, rightParamValue, twine, false, false);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
    }
    
    bool visitUnaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, const llvm::Twine&), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *paramValue = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &paramValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(paramValue, twine);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
    }
    
    bool visitUnaryOperatorNode(ASTNode node, llvm::Value **outValue, llvm::Value *(llvm::IRBuilder<>::*llvmFunction)(llvm::Value*, const llvm::Twine&, bool, bool), const llvm::Twine& twine, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *paramValue = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &paramValue, ctx);
        
        llvm::Value *storedValue = (ctx->getIRBuilder()->*llvmFunction)(paramValue, twine, false, false);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
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
        return visitUnaryOperatorNode(node,
                                      outValue,
                                      &llvm::IRBuilder<>::CreateNot,
                                      "nottmp",
                                      walker,
                                      ctx);
    }

    bool visitLogicalOrExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitMinusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) == 1)
            return visitUnaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateNeg, "negtmp", walker, ctx);
        
        return visitBinaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateSub, "subtmp", walker, ctx);
    }
    
    bool visitModuloExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node, outValue,
                                       &llvm::IRBuilder<>::CreateSRem,
                                       "modtmp",
                                       walker,
                                       ctx);
    }
    
    bool visitMultiplyExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateMul,
                                       "multmp",
                                       walker,
                                       ctx);
    }
    
    bool visitNotEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return visitBinaryOperatorNode(node,
                                       outValue,
                                       &llvm::IRBuilder<>::CreateICmpEQ,
                                       "netmp",
                                       walker,
                                       ctx);
    }

    bool visitNullStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return true;
    }

    bool visitNumberLiteralNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::stringstream strVal;
        strVal.str(Utilities::getNodeText(node));   

        int64_t val;
        strVal >> val;
        
        if (outValue)
            *outValue = ConstantInt::get(Utilities::getLLVMTypeFromType(Type(PrimitiveTypeNumber)), val);
        
        return true;
    }

    bool visitNumberTypeNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitParamsNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
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

    bool visitPlusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(node) == 1) {
            // TODO: Implement absolute value transform.
            
            return true;
        }
        
        return visitBinaryOperatorNode(node, outValue, &llvm::IRBuilder<>::CreateSub, "addtmp", walker, ctx);
    }

    bool visitProcedureDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
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
            if (!walker->visitNode(nodeI1, NULL, ctx))
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
        
        bool result = walker->visitNode(bodyNode, NULL, ctx);
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

    bool visitPrintStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        std::vector<llvm::Type*> parameterTypes;
        parameterTypes.push_back(llvm::Type::getInt8PtrTy(getGlobalContext()));
        
        FunctionType *printfFunctionType = FunctionType::get(llvm::Type::getInt32Ty(getGlobalContext()),
                                                             parameterTypes,
                                                             true);
        
        Function *printfFunction = cast<Function>(ctx->getModule()->getOrInsertFunction("printf", printfFunctionType));
        
        llvm::Value *printVal = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &printVal, ctx);
        
        Type type;
        Utilities::getTypeFromExpressionNode(Utilities::getChildNodeAtIndex(node, 0),
                                             &type,
                                             false,
                                             walker,
                                             ctx,
                                             NULL);
        
        llvm::Value *formatStringValue = ctx->ioFormatStringForExpressionType(type);
        
        // Create the printf() call.
        ctx->getIRBuilder()->CreateCall2(printfFunction, formatStringValue, printVal);
        
        return true;
    }

    bool visitProcFuncInvocationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        Entity *entity = NULL;
        if (!ctx->isSymbolInScope(identifier, &entity))
            return false;
        
        std::vector<llvm::Value*> arguments;

        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(identifierNode); ++i) {
            llvm::Value *value = NULL;
            walker->visitNode(Utilities::getChildNodeAtIndex(identifierNode, i), &value, ctx);
            // Load the value into an argument
            llvm::Value *arg = ctx->getIRBuilder()->CreateLoad(value);
            arguments.push_back(arg);
        }
        
        FunctionProcedureEntity *funcProcEntity = dynamic_cast<FunctionProcedureEntity*>(entity);
        
        llvm::ArrayRef<llvm::Value*> a(&arguments[0], arguments.size());
        
        llvm::Value * v = ctx->getIRBuilder()->CreateCall(funcProcEntity->getLLVMFunction(), a, "calltmp");
        if (outValue)
            *outValue = v;
        return true;
    }

    bool visitProgramNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitReturnStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *returnValue = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &returnValue, ctx);
        
        ctx->getIRBuilder()->CreateRet(returnValue);
        
        return true;
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
        std::string strVal = Utilities::getNodeText(node);   

        if (outValue)
            *outValue = ctx->getIRBuilder()->CreateGlobalStringPtr(strVal.c_str(), "string");
        
        return true;
    }

    bool visitTildeExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitVariableDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!ctx->getCurrentFunctionProcedureEntity())
            return visitVariableDeclarationNodeAsGlobalVariable(node, outValue, walker, ctx);
        
        return visitVariableDeclarationNodeAsLocalVariable(node, outValue, walker, ctx);
    }
    
    bool visitVariableDeclarationNodeAsGlobalVariable(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
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
        
        if (outValue)
            *outValue = value;
        
        return true;
    }
    
    bool visitVariableDeclarationNodeAsLocalVariable(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        std::string typeString = Utilities::getNodeText(typeNode);
        
        ASTNode valueNode = Utilities::getChildNodeAtIndex(node, 2);
        
        VariableEntity *variable = new VariableEntity(identifier,
                                                      Utilities::getNodeLineNumber(identifierNode),
                                                      Utilities::getTypeFromTypeString(typeString));
        llvm::Value *value = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromType(variable->getType()),
                                                               0,
                                                               identifier.c_str());
        variable->setLLVMValue(value);
        ctx->addEntityInScope(identifier, variable);
        
        if (valueNode) {
            llvm::Value *assignmentValue = NULL;
            if (walker->visitNode(valueNode, &assignmentValue, ctx))
                ctx->getIRBuilder()->CreateStore(assignmentValue, value);
        }
        
        if (outValue)
            *outValue = value;
        
        return true;
    }
    
    bool visitWhileStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return true;
        
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