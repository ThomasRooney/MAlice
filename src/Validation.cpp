
#include "Validation.h"

#include "ASTWalker.h"
#include "CompilerContext.h"
#include "ErrorReporter.h"
#include "ErrorFactory.h"
#include "FunctionEntity.h"
#include "Utilities.h"
#include "ProcedureEntity.h"
#include "SemanticChecks.h"
#include "VariableEntity.h"

namespace MAlice {


    // Separator comment (for git).
    
    bool Validation::validateFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
                return false;
            
            // Get the return type
            MAliceType returnType = MAliceType::MAliceTypeNone;
            //
            bool hasParams = false;
            // get node index 1, if its a parameter node, get params...
            ASTNode nodeI1 = Utilities::getChildNodeAtIndex(node, 1);
            if (Utilities::getNodeType(nodeI1) == PARAMS)
                hasParams = true;
            ASTNode returnNode = Utilities::getChildNodeAtIndex(node, hasParams?2:1);
            if (returnNode != NULL)
                returnType = Utilities::getTypeFromTypeString(Utilities::getNodeText(returnNode));
            
            FunctionEntity *functionEntity = new FunctionEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::list<ParameterEntity>(), returnType);
            if (hasParams)
            {
                std::list<ParameterEntity> parameterList = getParameterTypesFromParamsNode(nodeI1);
                functionEntity->setParameterListTypes(parameterList);
                
                for (auto p = parameterList.begin(); p!=  parameterList.end();p++) {
                    ctx->addEntityInScope(p->getIdentifier(), p->clone());
                }
            }
            ctx->addEntityInScope(identifier, functionEntity);
            ctx->pushFunctionProcedureEntity(functionEntity);
            
            bool result = visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(node, functionEntity, walker, ctx);
            
            ctx->popFunctionProcedureEntity();
            
            return result;
        }
        
        return true;
    }
    
    bool Validation::validateProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        if (!checkSymbolForInvocationIsValidOrOutputError(node, walker, ctx))
            return false;
        
        // This is being called in the context of a statement, not within an expression.
        if (getReturnTypeForInvocation(node, walker, ctx) != MAliceTypeNone) {
            FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(node, walker, ctx);
            std::string identifier = funcProcEntity->getIdentifier();
            
            Range *range = NULL;
            Utilities::getNodeTextIncludingChildren(node, ctx, &range);
            
            Error *error = ErrorFactory::createWarningError("Unused return value from function '" + identifier + "'.");
            
            ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
            if (identifierNode) {
                error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
                error->setUnderlineRanges(Utilities::rangeToSingletonList(range));
            }
            
            ctx->getErrorReporter()->reportError(error);
        }
        
        if (!checkNumberOfArgumentsForInvocationIsValid(node, walker, ctx))
            return false;
        
        if (!checkTypesOfArgumentsForInvocationIsValid(node, walker, ctx))
            return false;
        
        return true;
    }
    
    bool Validation::validateProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier = Utilities::getNodeText(identifierNode);
            
            if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
                return false;
            
            ProcedureEntity *procedureEntity = new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), std::list<ParameterEntity>());
            
            ctx->addEntityInScope(identifier, procedureEntity);
            ctx->pushFunctionProcedureEntity(procedureEntity);
            
            bool result = visitIntoFunctionProcedureChildNodesAndPopulateSymbolTableEntity(node, procedureEntity, walker, ctx);
            
            ctx->popFunctionProcedureEntity();
            
            return result;
        }
        
        return true;
    }
    
    bool Validation::validateVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (ctx->isKeyword(identifier)) {
                Error *error = ErrorFactory::createSemanticError("Cannot declare variable '" + identifier + "' because it is a keyword.");
                error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
                
                error->setArrowRanges(Utilities::rangeToSingletonList(
                                                                      Utilities::createRange(Utilities::getNodeLineNumber(identifierNode), Utilities::getNodeColumnIndex(identifierNode))
                                                                      ));
                
                ctx->getErrorReporter()->reportError(error);
                
                return false;
            }
            
            checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx);
            
            MAliceType type = Utilities::getTypeFromTypeString(std::string(Utilities::getNodeText(typeNode)));
            
            ctx->addEntityInScope(identifier, new VariableEntity(identifier, Utilities::getNodeLineNumber(identifierNode), type));
            
            // See if a third node exists - if so this is a variable assignment node that we want to visit
            
            ASTNode thirdNode = Utilities::getChildNodeAtIndex(node, 2);
            if (thirdNode != NULL)
                checkExpression(thirdNode, walker, ctx, type);
            
        }
        
        return true;
    }


}
