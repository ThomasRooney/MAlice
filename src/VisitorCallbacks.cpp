#include <string>
#include <sstream>
#include <list>

#include "VisitorCallbacks.h"
#include "Entity.h"
#include "FunctionEntity.h"
#include "ProcedureEntity.h"
#include "VariableEntity.h"
#include "Utilities.h"

#define SSTR( x ) dynamic_cast< std::ostringstream & > \
        ((( std::ostringstream() << std::dec << x ) ).str())

namespace MAlice {

    // Statements
    bool visitNullStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitPrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitStatementListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    // Expressions
    bool visitLogicalAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitLogicalOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitLogicalNotExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitBitwiseAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitBitwiseOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitBitwiseXorExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitDivideExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitEqualsExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitGreaterThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitGreaterThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitIdentifierNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitLessThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitLessThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitMinusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitModuloExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitMultiplyExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitNotEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitPlusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitTildeExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    // Imaginary nodes – used to improve AST structure
    bool visitArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            std::string type;
            checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx);

            // array of what?
            ASTNode typeNode = Utilities::getChildNodeAtIndex(identifierNode, 0);
            if (typeNode != NULL)
                type = Utilities::getNodeText(typeNode);
            
            
            ctx->addEntityInScope(identifier, new VariableEntity(identifier, Utilities::getNodeLineNumber(node), Utilities::getTypeFromTypeString(type)));
        }
    }
    
    bool visitArraySubscriptNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
    }
    
    bool visitBodyNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        walker->visitChildren(node, ctx);
    }
    
    bool visitByReferenceParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
      walker->visitChildren(node, ctx);
    }
    
    bool visitByValueParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
      walker->visitChildren(node, ctx);
    }
    
    bool visitDeclarationsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        walker->visitChildren(node, ctx);
    }
    
    bool visitArithmeticExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    bool visitBooleanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    bool visitExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    bool visitFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
                return false;
            
            std::list<ParameterEntity> parameterList = visitIntoFunctionProcedureScope(node,walker,ctx);
            
            ctx->addEntityInScope(identifier, new FunctionEntity(identifier, Utilities::getNodeLineNumber(identifierNode), parameterList, MAliceTypeUndefined));
        }
        
        return true;
    }
    
    bool visitProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode == NULL)
            return false;
        
        std::string identifier(Utilities::getNodeText(identifierNode));
        
        Entity *entityForIdentifier = NULL;
        if (!ctx->isSymbolInScope(identifier, &entityForIdentifier)) {
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                 Utilities::getNodeColumnIndex(identifierNode),
                                                 ErrorType::Semantic,
                                                 "Cannot find procedure or function declaration for '" + identifier + "'.",
                                                 true);
            
            return false;
        }
        
        return true;
    }
    
    bool visitParamsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier;
        
        if (identifierNode != NULL) {
            identifier = Utilities::getNodeText(identifierNode);
            
            if (!checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx))
                return false;
        }
        std::list<ParameterEntity> parameterList = visitIntoFunctionProcedureScope(node,walker,ctx);
        
        ctx->addEntityInScope(identifier, new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), parameterList));
        
        return true;
    }
    
    bool visitVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (ctx->isKeyword(identifier)) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                     Utilities::getNodeColumnIndex(identifierNode),
                                                     ErrorType::Semantic,
                                                     "Cannot declare variable '" + identifier + "' because it is a keyword.",
                                                     true);
                
                return false;
            }
            
            checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx);
            
            MAliceType type = Utilities::getTypeFromTypeString(std::string(Utilities::getNodeText(typeNode)));
            ctx->addEntityInScope(identifier, new VariableEntity(identifier, Utilities::getNodeLineNumber(identifierNode), type));
        }
        
        return true;
    }
    
    // Literals
    bool visitCharacterLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitNumberLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitStringLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    // Types
    bool visitLetterTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitNumberTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }
    
    bool visitSentenceTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        return walker->visitChildren(node, ctx);
    }


    // Helper methods
    bool checkSymbolNotInCurrentScopeOrOutputError(std::string identifier, ASTNode node, CompilerContext *ctx)
    {
        Entity *existingEntity = NULL;
        
        if (ctx->isSymbolInCurrentScope(identifier, &existingEntity)) {
            std::stringstream errorMessage;
            errorMessage << "'" << identifier << "' has already been declared in the current scope."
            << "\n  - Declared as a " << existingEntity->humanReadableName() << " on line " << existingEntity->getLineNumber();
            
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                 Utilities::getNodeColumnIndex(node),
                                                 ErrorType::Semantic,
                                                 errorMessage.str(),
                                                 true);
            
            return false;
        }
        
        return true;
    }
    
    std::list<ParameterEntity> getParameterTypesFromParamsNode(ASTNode paramsNode)
    {
        std::list<ParameterEntity> parameterTypes;
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(paramsNode); ++i) {
            ASTNode childNode = Utilities::getChildNodeAtIndex(paramsNode, i);
            bool passedByReference = (Utilities::getNodeType(childNode) == BYREFERENCE);
            
            ASTNode passTypeNode = Utilities::getChildNodeAtIndex(childNode, 1);
            std::string identifier = Utilities::getNodeText(passTypeNode);
            int lineNumber = Utilities::getNodeLineNumber(passTypeNode);
            ASTNode typeNode = Utilities::getChildNodeAtIndex(childNode, 0);
            
            std::string typeString = Utilities::getNodeText(typeNode);
            MAliceType t = (Utilities::getTypeFromTypeString(typeString));
            ParameterEntity p = ParameterEntity(identifier, lineNumber, t, passedByReference);

            parameterTypes.push_back(p);
        }
        
        return parameterTypes;
    }

    std::list<ParameterEntity> visitIntoFunctionProcedureScope(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        std::list<ParameterEntity> parameterList;
        ctx->enterScope();
        // Loop through the rest of the child nodes
        for (unsigned int i = 1; i < Utilities::getNumberOfChildNodes(node); ++i) {
            ASTNode childNode = Utilities::getChildNodeAtIndex(node, i);            
                        
            switch (Utilities::getNodeType(childNode))
            {
                case PARAMS:
                    parameterList = getParameterTypesFromParamsNode(childNode);
                    for (auto p = parameterList.begin(); p!=  parameterList.end();p++)
                    {
                        ctx->addEntityInScope(p->getIdentifier(), p->clone());
                    }
                    break;
                case BODY:
                    walker->visitNode(childNode, ctx);
                    break;
                default:
                    break;
            }
        }
        ctx->exitScope();
        return parameterList;
    }
};