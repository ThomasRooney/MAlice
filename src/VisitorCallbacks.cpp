#include <string>
#include <sstream>
#include <list>
#include <typeinfo>

#include "VisitorCallbacks.h"
#include "ArrayEntity.h"
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
        
        Function *function = NULL;
        BasicBlock *insertBlock = ctx->getIRBuilder()->GetInsertBlock();
        if (insertBlock)
            function = insertBlock->getParent();
        
        BasicBlock *enterBlock = BasicBlock::Create(getGlobalContext(), "_opened", function);
        ctx->getIRBuilder()->SetInsertPoint(enterBlock);
                                                    
        bool result = walker->visitChildren(node, NULL, ctx);

        ctx->exitScope();
        BasicBlock *exitBlock = BasicBlock::Create(getGlobalContext(), "_closed", function);
        ctx->getIRBuilder()->SetInsertPoint(exitBlock);
        
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
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ASTNode typeNode = Utilities::getChildNodeAtIndex(identifierNode, 0);
        std::string typeString = Utilities::getNodeText(typeNode);
        
        ArrayEntity *arrayEntity = new ArrayEntity(identifier, Utilities::getNodeLineNumber(node), Utilities::getTypeFromTypeString(typeString), 1);
        ctx->addEntityInScope(identifier, arrayEntity);
        
        llvm::Value *value = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromMAliceType(arrayEntity->getType()));
        arrayEntity->setLLVMValue(value);
        
        return true;
    }

    bool visitArraySubscriptNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
    }

    bool visitAssignmentStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateAssignmentStatementNode(node, walker, ctx))
            return false;
        
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
        std::string strVal = Utilities::getNodeText(node);
        uint64_t val = strVal[1]; // [0] = "'", [1] = LITERAL, [2] = "'"
        
        if (outValue)
            *outValue = ConstantInt::get(Utilities::getLLVMTypeFromMAliceType(MAliceTypeLetter), val);
        
        return true;
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
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
        
        llvm::Value *storedValue = ctx->getIRBuilder()->CreateICmpEQ(leftParamValue, rightParamValue);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
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
        std::vector<Type*> parameterTypes;
        for (auto it = parameterEntities.begin(); it != parameterEntities.end(); ++it) {
            ParameterEntity *entity = *it;
            parameterTypes.push_back(Utilities::getLLVMTypeFromMAliceType(entity->getType()));
        }
        
        FunctionType *functionType = FunctionType::get(Utilities::getLLVMTypeFromMAliceType(functionEntity->getReturnType()),
                                                       parameterTypes,
                                                       false);
        
        Function *function = Function::Create(functionType,
                                              Function::InternalLinkage,
                                              identifier.c_str(),
                                              ctx->getModule());
        
        functionEntity->setLLVMFunction(function);
        
//        for (auto it = function->arg_begin(); i < parameterEntities.size(); ++it) {
////            ParameterEntity *entity = parameterEntities.at(i);
//            if (it)
//                it->setName(entity->getIdentifier().c_str());
//            
//            ++i;
//        }
        
        
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
        return walker->visitChildren(node, NULL, ctx);
    }
    
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, NULL, ctx);
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
            *outValue = ctx->getIRBuilder()->CreateLoad(value, identifier.c_str());
        
        return true;
    }

    bool visitIfStatementNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateIfStatementNode(node, walker, ctx))
            return false;
        
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
        
        llvm::Value *condValue = NULL;
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &condValue, ctx);
        
        BasicBlock *thenBlock = BasicBlock::Create(getGlobalContext(), "then", function);
        BasicBlock *afterBlock = BasicBlock::Create(getGlobalContext(), "after", function);
        
        ctx->getIRBuilder()->CreateCondBr(condValue, thenBlock, afterBlock);
        
        return true;
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
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
        
        llvm::Value *storedValue = ctx->getIRBuilder()->CreateICmpULT(leftParamValue, rightParamValue);
        
        if (outValue)
            *outValue = storedValue;
        
        return true;
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
        unsigned int numChildren = Utilities::getNumberOfChildNodes(node);
        
        llvm::Value *leftParamValue = NULL;
        llvm::Value *storedValue = NULL;
        
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        
        if (numChildren == 2) {
            llvm::Value *rightParamValue = NULL;
            walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
            storedValue = ctx->getIRBuilder()->CreateFSub(leftParamValue, rightParamValue, "subtmp");
        }
        else
            storedValue = ctx->getIRBuilder()->CreateNeg(leftParamValue);
        
        if (outValue)
            *outValue = storedValue;

        return true;
    }
    
    bool visitModuloExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return true;
        
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
                
        llvm::Value *storedValue = ctx->getIRBuilder()->CreateSRem(leftParamValue, rightParamValue, "modtmp");
        
        if (outValue)
            *outValue = storedValue;
        

        return true;

    }
    
    bool visitMultiplyExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
                
        llvm::Value *storedValue = ctx->getIRBuilder()->CreateFMul(leftParamValue, rightParamValue, "multmp");
        
        if (outValue)
            *outValue = storedValue;
        

        return true;
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

        int64_t val;
        strVal >> val;
        
        if (outValue)
            *outValue = ConstantInt::get(Utilities::getLLVMTypeFromMAliceType(MAliceTypeNumber), val);
        
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
            llvm::Value *parameterValue = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromMAliceType(entity->getType()), 0, entity->getIdentifier().c_str());
            entity->setLLVMValue(parameterValue);
            
            ctx->addEntityInScope(entity->getIdentifier(), entity);
        }
        
        entity->setParameterListTypes(parameterList);
        
        return true;
    }

    bool visitPlusExpressionNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        return true;
        
        llvm::Value *leftParamValue = NULL;
        llvm::Value *rightParamValue = NULL;
        
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 0), &leftParamValue, ctx);
        walker->visitNode(Utilities::getChildNodeAtIndex(node, 1), &rightParamValue, ctx);
                
        llvm::Value *storedValue = ctx->getIRBuilder()->CreateFAdd(leftParamValue, rightParamValue, "addtmp");
        
        if (outValue)
            *outValue = storedValue;
        

        return true;
    }

    bool visitProcedureDeclarationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateProcedureDeclarationNode(node, walker, ctx))
            return false;
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        ProcedureEntity *procedureEntity = new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::vector<ParameterEntity*>());
        std::string LLVMIdentifier = ctx->getIdentifierDispenser()->identifierForFunctionWithName(identifier);
        
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
        std::vector<Type*> parameterTypes;
        std::vector<ParameterEntity*> parameterEntities = procedureEntity->getParameterListTypes();
        
        for (auto it = parameterEntities.begin(); it != parameterEntities.end(); ++it) {
            ParameterEntity *entity = *it;
            parameterTypes.push_back(Utilities::getLLVMTypeFromMAliceType(entity->getType()));
        }
        
        ArrayRef<Type*> parameterArrayRefs = makeArrayRef(parameterTypes);
        FunctionType *procedureType = FunctionType::get(Type::getVoidTy(getGlobalContext()),
                                                        parameterArrayRefs,
                                                        false);
        
        Function *procedure = Function::Create(procedureType,
                                               Function::InternalLinkage,
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
        
        if (outValue)
            *outValue = procedure;
        
        return true;
    }

    bool visitProcFuncInvocationNode(ASTNode node, llvm::Value **outValue, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!Validation::validateProcFuncInvocationNode(node, walker, ctx))
            return false;
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier = Utilities::getNodeText(identifierNode);
        
        Entity *entity = NULL;
        ctx->isSymbolInScope(identifier, &entity);
        
        std::vector<llvm::Value*> arguments;
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(identifierNode); ++i) {
            llvm::Value *value = NULL;
            walker->visitNode(Utilities::getChildNodeAtIndex(identifierNode, i), &value, ctx);
            
            arguments.push_back(value);
        }
        
        FunctionProcedureEntity *funcProcEntity = dynamic_cast<FunctionProcedureEntity*>(entity);
        ctx->getIRBuilder()->CreateCall(funcProcEntity->getLLVMFunction(), arguments);
        
        return true;
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
        llvm::Value *value = new GlobalVariable(*(ctx->getModule()),
                                                Utilities::getLLVMTypeFromMAliceType(variable->getType()),
                                                false,
                                                GlobalValue::LinkerPrivateLinkage,
                                                NULL,
                                                Twine(identifier.c_str()));
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
        llvm::Value *value = ctx->getIRBuilder()->CreateAlloca(Utilities::getLLVMTypeFromMAliceType(variable->getType()),
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
        if (!Validation::validateWhileStatementNode(node, walker, ctx))
            return false;
        
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