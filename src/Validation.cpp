
#include "Validation.h"

#include "ASTWalker.h"
#include "CompilerContext.h"
#include "ErrorReporter.h"
#include "ErrorFactory.h"
#include "Utilities.h"
#include "ProcedureEntity.h"
#include "SemanticChecks.h"
#include "VariableEntity.h"

namespace MAlice {


    // Separator comment (for git).
    
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
