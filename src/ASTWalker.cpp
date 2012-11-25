#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <string>
#include <sstream>

#include "ASTWalker.h"

#include "MAliceParser.h"
#include "SymbolTable.h"
#include "Utilities.h"
#include "VisitorCallbacks.h"

namespace MAlice {

ASTWalker :: ASTWalker () {
  constructVisitDictionary();
}
    
ASTWalker :: ~ASTWalker() {
}

void ASTWalker :: constructVisitDictionary() {
  static bool doOnce = false;
  if (!doOnce) {
      doOnce = true;
      visitDictionary = std::unordered_map<unsigned int, MAliceVisitFunction>();
      visitDictionary.insert(std::make_pair(ARRAY, &visitArrayDeclarationNode));
      visitDictionary.insert(std::make_pair(ARRAYSUBSCRIPT, &visitArraySubscriptNode));
      visitDictionary.insert(std::make_pair(ASSIGNMENTSTATEMENT, &visitAssignmentStatementNode));
      visitDictionary.insert(std::make_pair(BANG, &visitLogicalNotExpressionNode));
      visitDictionary.insert(std::make_pair(BITWISEAND, &visitBitwiseAndExpressionNode));
      visitDictionary.insert(std::make_pair(BITWISEOR, &visitBitwiseOrExpressionNode));
      visitDictionary.insert(std::make_pair(BITWISEXOR, &visitBitwiseXorExpressionNode));
      visitDictionary.insert(std::make_pair(BODY, &visitBodyNode));
      visitDictionary.insert(std::make_pair(ARRAY, &visitArrayDeclarationNode));
      visitDictionary.insert(std::make_pair(BOOLEANEXPRESSION, &visitBooleanExpressionNode));
      visitDictionary.insert(std::make_pair(BYREFERENCE, &visitByReferenceParameterNode));
      visitDictionary.insert(std::make_pair(BYREFERENCE, &visitByValueParameterNode));
      visitDictionary.insert(std::make_pair(CHARACTER_LITERAL, &visitCharacterLiteralNode));
      visitDictionary.insert(std::make_pair(DECLS, &visitDeclarationsNode));
      visitDictionary.insert(std::make_pair(DECREMENTSTATEMENT, &visitDecrementStatementNode));
      visitDictionary.insert(std::make_pair(DIVIDE, &visitDivideExpressionNode));
      visitDictionary.insert(std::make_pair(EQUALS, &visitEqualsExpressionNode));
      visitDictionary.insert(std::make_pair(EXPRESSION, &visitExpressionNode));
      visitDictionary.insert(std::make_pair(FULL_STOP, &visitNullStatementNode));
      visitDictionary.insert(std::make_pair(FUNCDEFINITION, &visitFunctionDeclarationNode));
      visitDictionary.insert(std::make_pair(GREATERTHAN, &visitGreaterThanExpressionNode));
      visitDictionary.insert(std::make_pair(GREATERTHANEQUAL, &visitGreaterThanOrEqualExpressionNode));
      visitDictionary.insert(std::make_pair(IDENTIFIER, &visitIdentifierNode));
      visitDictionary.insert(std::make_pair(IFSTATEMENT, &visitIfStatementNode));
      visitDictionary.insert(std::make_pair(INCREMENTSTATEMENT, &visitIncrementStatementNode));
      visitDictionary.insert(std::make_pair(INPUTSTATEMENT, &visitInputStatementNode));
      visitDictionary.insert(std::make_pair(INVOCATION, &visitProcFuncInvocationNode));
      visitDictionary.insert(std::make_pair(LESSTHAN, &visitLessThanExpressionNode));
      visitDictionary.insert(std::make_pair(LESSTHANEQUAL, &visitLessThanOrEqualExpressionNode));
      visitDictionary.insert(std::make_pair(LETTER_TYPE, &visitLetterTypeNode));
      visitDictionary.insert(std::make_pair(LOGICALAND, &visitLogicalAndExpressionNode));
      visitDictionary.insert(std::make_pair(LOGICALOR, &visitLogicalOrExpressionNode));
      visitDictionary.insert(std::make_pair(MINUS, &visitMinusExpressionNode));
      visitDictionary.insert(std::make_pair(MODULO, &visitModuloExpressionNode));
      visitDictionary.insert(std::make_pair(MULTIPLY, &visitMultiplyExpressionNode));
      visitDictionary.insert(std::make_pair(NOTEQUAL, &visitNotEqualExpressionNode));
      visitDictionary.insert(std::make_pair(NUMBER_LITERAL, &visitNumberLiteralNode));
      visitDictionary.insert(std::make_pair(NUMBER_TYPE, &visitNumberTypeNode));
      visitDictionary.insert(std::make_pair(PARAMS, &visitParamsNode));
      visitDictionary.insert(std::make_pair(PLUS, &visitPlusExpressionNode));
      visitDictionary.insert(std::make_pair(PRINTSTATEMENT, &visitPrintStatementNode));
      visitDictionary.insert(std::make_pair(PROCDEFINITION, &visitProcedureDeclarationNode));
      visitDictionary.insert(std::make_pair(RETURNSTATEMENT, &visitReturnStatementNode));
      visitDictionary.insert(std::make_pair(SENTENCE_TYPE, &visitSentenceTypeNode));
      visitDictionary.insert(std::make_pair(STATEMENTLIST, &visitStatementListNode));
      visitDictionary.insert(std::make_pair(STRING_LITERAL, &visitStringLiteralNode));
      visitDictionary.insert(std::make_pair(TILDE, &visitLogicalNotExpressionNode));
      visitDictionary.insert(std::make_pair(VARDECLARATION, &visitVariableDeclarationNode));
      visitDictionary.insert(std::make_pair(WHILESTATEMENT, &visitWhileStatementNode));
      visitDictionary.insert(std::make_pair(BYVALUE, &visitByValueParameterNode));
      visitDictionary.insert(std::make_pair(BYREFERENCE, &visitByReferenceParameterNode));
  }
}

bool ASTWalker::validateTree(pANTLR3_BASE_TREE root, CompilerContext *ctx)
{
  return visitNode(root, ctx);
}


bool ASTWalker::visitNode(ASTNode node, CompilerContext *ctx)
{
    MAliceVisitFunction f = getNodeVisitFunction(node);
    
    // If we haven't implemented visitor functions for certain node types (e.g. nodes which we don't do anything
    // with but are there to make the AST nicer to work with), simply recurse on their children and vist them.
    if (f == NULL) {
        return visitChildren(node, ctx);
    }
    
    return f(node, this, ctx);
}
    
bool ASTWalker::visitChildren(ASTNode node, CompilerContext *ctx)
{
    unsigned int numChildren = Utilities::getNumberOfChildNodes(node);
    
    bool result = true;
    
    for (unsigned int i = 0; i < numChildren; ++i) {
        if (!visitNode(Utilities::getChildNodeAtIndex(node, i), ctx))
            result = false;
    }
    
    return result;
}
    
MAliceVisitFunction ASTWalker::getNodeVisitFunction(ASTNode node)
{
    MAliceVisitFunction f = NULL;
    
    try {
        f = (visitDictionary.at(Utilities::getNodeType(node)));
    }
    catch (std::out_of_range e) {
        return NULL;
    }
    
    return f;
}

void ASTWalker :: compileTree()
{

}

}; // namespace MAlice