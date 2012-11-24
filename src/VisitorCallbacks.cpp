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
    void visitNullStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitAssignmentStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitIncrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitDecrementStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitIfStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitInputStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitPrintStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitReturnStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitStatementListNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitWhileStatementNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    // Expressions
    void visitLogicalAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitLogicalOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitLogicalNotExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBitwiseAndExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBitwiseOrExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBitwiseXorExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitDivideExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitEqualsExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitGreaterThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitGreaterThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitIdentifierNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
        // Do nothing here, extracted directly.
    }
    
    void visitLessThanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitLessThanOrEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitMinusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitModuloExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitMultiplyExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitNotEqualExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitPlusExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitTildeExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    // Imaginary nodes – used to improve AST structure
    void visitArrayDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
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
    
    void visitArraySubscriptNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBodyNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        walker->visitChildren(node, ctx);
    }
    
    void visitByReferenceParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
      walker->visitChildren(node, ctx);
    }
    
    void visitByValueParameterNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
      walker->visitChildren(node, ctx);
    }
    
    void visitDeclarationsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        walker->visitChildren(node, ctx);
    }
    
    void visitArithmeticExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitBooleanExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitExpressionNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitFunctionDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (identifierNode != NULL) {
            std::string identifier((char*)identifierNode->toString(identifierNode)->chars);
            
            checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx);
            std::list<ParameterEntity> parameterList = visitIntoFunctionProcedureScope(node,walker,ctx);
            
            ctx->addEntityInScope(identifier, new FunctionEntity(identifier, Utilities::getNodeLineNumber(identifierNode), parameterList, MAliceTypeUndefined));
        }
        
        
    }
    
    void visitProcFuncInvocationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitParamsNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
      walker->visitChildren(node, ctx);
    }
    
    void visitProcedureDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        std::string identifier;
        
        if (identifierNode != NULL) {
            std::string identifier((char*)identifierNode->toString(identifierNode)->chars);
            
            checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx);
        }
        std::list<ParameterEntity> parameterList = visitIntoFunctionProcedureScope(node,walker,ctx);
        
        ctx->addEntityInScope(identifier, new ProcedureEntity(identifier, Utilities::getNodeLineNumber(identifierNode), parameterList));
    }
    
    void visitVariableDeclarationNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
    {
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
        ASTNode typeNode = Utilities::getChildNodeAtIndex(node, 1);
        
        if (identifierNode != NULL) {
            std::string identifier(Utilities::getNodeText(identifierNode));
            
            if (ctx->isKeyword(identifier)) {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(identifierNode),
                                                     Utilities::getNodeColumnIndex(identifierNode),
                                                     ErrorTypeSemantic,
                                                     "Cannot declare variable '" + identifier + "' because it is a keyword.",
                                                     true);
            }
            
            checkSymbolNotInCurrentScopeOrOutputError(identifier, identifierNode, ctx);
            
            MAliceType type = Utilities::getTypeFromTypeString(std::string(Utilities::getNodeText(typeNode)));
            ctx->addEntityInScope(identifier, new VariableEntity(identifier, Utilities::getNodeLineNumber(identifierNode), type));
        }
    }
    
    // Literals
    void visitCharacterLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitNumberLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitStringLiteralNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    // Types
    void visitLetterTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitNumberTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }
    
    void visitSentenceTypeNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx) {
    }


    // Helper methods
    void checkSymbolNotInCurrentScopeOrOutputError(std::string identifier, ASTNode node, CompilerContext *ctx)
    {
        Entity *existingEntity = NULL;
        
        if (ctx->isSymbolInScope(identifier, &existingEntity)) {
            std::stringstream errorMessage;
            errorMessage << "'" << identifier << "' has already been declared in the current scope."
            << "\n  - Declared as a " << existingEntity->humanReadableName() << " on line " << existingEntity->getLineNumber();
            
            ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                 Utilities::getNodeColumnIndex(node),
                                                 ErrorTypeSemantic,
                                                 errorMessage.str(),
                                                 true);
        }
    }
    
    std::list<ParameterEntity> getParameterTypesFromParamsNode(ASTNode paramsNode)
    {
        std::list<ParameterEntity> parameterTypes;
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(paramsNode); ++i) {
            ASTNode childNode = Utilities::getChildNodeAtIndex(paramsNode, i);
            bool passedByReference = (childNode->getType(childNode) == BYREFERENCE);
            
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