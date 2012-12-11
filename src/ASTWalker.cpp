#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <string>
#include <sstream>

#include "ASTWalker.h"

#include "Validation.h"

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


// Initialise the static function dictionaries
std::unordered_map<unsigned int, MAliceVisitFunction> ASTWalker::visitDictionary = std::unordered_map<unsigned int, MAliceVisitFunction>();
std::unordered_map<unsigned int, MAliceValidationFunction> ASTWalker::validationDictionary = std::unordered_map<unsigned int, MAliceValidationFunction>();

void ASTWalker :: constructVisitDictionary() {
    static bool doOnce = false;
    if (!doOnce) {
        doOnce = true;
        ASTWalker::visitDictionary.insert(std::make_pair(ARRAY, &visitArrayDeclarationNode));
        ASTWalker::visitDictionary.insert(std::make_pair(ARRAYSUBSCRIPT, &visitArraySubscriptNode));
        ASTWalker::visitDictionary.insert(std::make_pair(ASSIGNMENTSTATEMENT, &visitAssignmentStatementNode));
        ASTWalker::visitDictionary.insert(std::make_pair(BANG, &visitLogicalNotExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(BITWISEAND, &visitBitwiseAndExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(BITWISEOR, &visitBitwiseOrExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(BITWISEXOR, &visitBitwiseXorExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(BLOCK, &visitArbitraryBlockNode));
        ASTWalker::visitDictionary.insert(std::make_pair(BODY, &visitBodyNode));
        ASTWalker::visitDictionary.insert(std::make_pair(BYREFERENCE, &visitByReferenceParameterNode));
        ASTWalker::visitDictionary.insert(std::make_pair(BYVALUE, &visitByValueParameterNode));
        ASTWalker::visitDictionary.insert(std::make_pair(CHARACTER_LITERAL, &visitCharacterLiteralNode));
        ASTWalker::visitDictionary.insert(std::make_pair(DECLS, &visitDeclarationsNode));
        ASTWalker::visitDictionary.insert(std::make_pair(DECREMENTSTATEMENT, &visitDecrementStatementNode));
        ASTWalker::visitDictionary.insert(std::make_pair(DIVIDE, &visitDivideExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(EQUALS, &visitEqualsExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(EXPRESSION, &visitExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(FULL_STOP, &visitNullStatementNode));
        ASTWalker::visitDictionary.insert(std::make_pair(FUNCDEFINITION, &visitFunctionDeclarationNode));
        ASTWalker::visitDictionary.insert(std::make_pair(GREATERTHAN, &visitGreaterThanExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(GREATERTHANEQUAL, &visitGreaterThanOrEqualExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(IDENTIFIER, &visitIdentifierNode));
        ASTWalker::visitDictionary.insert(std::make_pair(IFSTATEMENT, &visitIfStatementNode));
        ASTWalker::visitDictionary.insert(std::make_pair(INCREMENTSTATEMENT, &visitIncrementStatementNode));
        ASTWalker::visitDictionary.insert(std::make_pair(INPUTSTATEMENT, &visitInputStatementNode));
        ASTWalker::visitDictionary.insert(std::make_pair(INVOCATION, &visitProcFuncInvocationNode));
        ASTWalker::visitDictionary.insert(std::make_pair(LESSTHAN, &visitLessThanExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(LESSTHANEQUAL, &visitLessThanOrEqualExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(LETTER_TYPE, &visitLetterTypeNode));
        ASTWalker::visitDictionary.insert(std::make_pair(LOGICALAND, &visitLogicalAndExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(LOGICALOR, &visitLogicalOrExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(MINUS, &visitMinusExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(MODULO, &visitModuloExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(MULTIPLY, &visitMultiplyExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(NOTEQUAL, &visitNotEqualExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(NUMBER_LITERAL, &visitNumberLiteralNode));
        ASTWalker::visitDictionary.insert(std::make_pair(NUMBER_TYPE, &visitNumberTypeNode));
        ASTWalker::visitDictionary.insert(std::make_pair(PARAMS, &visitParamsNode));
        ASTWalker::visitDictionary.insert(std::make_pair(PLUS, &visitPlusExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(PRINTSTATEMENT, &visitPrintStatementNode));
        ASTWalker::visitDictionary.insert(std::make_pair(PROCDEFINITION, &visitProcedureDeclarationNode));
        ASTWalker::visitDictionary.insert(std::make_pair(PROGRAM, &visitProgramNode));
        ASTWalker::visitDictionary.insert(std::make_pair(RETURNSTATEMENT, &visitReturnStatementNode));
        ASTWalker::visitDictionary.insert(std::make_pair(SENTENCE_TYPE, &visitSentenceTypeNode));
        ASTWalker::visitDictionary.insert(std::make_pair(STATEMENTLIST, &visitStatementListNode));
        ASTWalker::visitDictionary.insert(std::make_pair(STRING_LITERAL, &visitStringLiteralNode));
        ASTWalker::visitDictionary.insert(std::make_pair(TILDE, &visitLogicalNotExpressionNode));
        ASTWalker::visitDictionary.insert(std::make_pair(VARDECLARATION, &visitVariableDeclarationNode));
        ASTWalker::visitDictionary.insert(std::make_pair(WHILESTATEMENT, &visitWhileStatementNode));
   
        ASTWalker::validationDictionary.insert(std::make_pair(ARRAY, &Validation::validateArrayDeclarationNode));
        ASTWalker::validationDictionary.insert(std::make_pair(ASSIGNMENTSTATEMENT, &Validation::validateAssignmentStatementNode));
        ASTWalker::validationDictionary.insert(std::make_pair(BODY, &Validation::validateBodyNode));
        ASTWalker::validationDictionary.insert(std::make_pair(BLOCK, &Validation::validateArbitraryBlockNode));
        ASTWalker::validationDictionary.insert(std::make_pair(DECREMENTSTATEMENT, &Validation::validateDecrementStatementNode));
        ASTWalker::validationDictionary.insert(std::make_pair(EXPRESSION, &Validation::validateExpressionNode));
        ASTWalker::validationDictionary.insert(std::make_pair(FUNCDEFINITION, &Validation::validateFunctionDeclarationNode));
        ASTWalker::validationDictionary.insert(std::make_pair(IFSTATEMENT, &Validation::validateIfStatementNode));
        ASTWalker::validationDictionary.insert(std::make_pair(INCREMENTSTATEMENT, &Validation::validateIncrementStatementNode));
        ASTWalker::validationDictionary.insert(std::make_pair(INPUTSTATEMENT, &Validation::validateInputStatementNode));
        ASTWalker::validationDictionary.insert(std::make_pair(INVOCATION, &Validation::validateProcFuncInvocationNode));
        ASTWalker::validationDictionary.insert(std::make_pair(PRINTSTATEMENT, &Validation::validatePrintStatementNode));
        ASTWalker::validationDictionary.insert(std::make_pair(PROCDEFINITION, &Validation::validateProcedureDeclarationNode));
        ASTWalker::validationDictionary.insert(std::make_pair(RETURNSTATEMENT, &Validation::validateReturnStatementNode));
        ASTWalker::validationDictionary.insert(std::make_pair(VARDECLARATION, &Validation::validateVariableDeclarationNode));
        ASTWalker::validationDictionary.insert(std::make_pair(WHILESTATEMENT, &Validation::validateWhileStatementNode));
        ASTWalker::validationDictionary.insert(std::make_pair(PARAMS, &Validation::validateParamsNode));
   
   
    }
}

bool ASTWalker::validateTree(pANTLR3_BASE_TREE root, CompilerContext *ctx)
{
  return validateNode(root, ctx);
}

bool ASTWalker::generateIRFromTree(pANTLR3_BASE_TREE root, CompilerContext *ctx)
{
    return visitNode(root, NULL, ctx);
}

bool ASTWalker::visitNode(ASTNode node, llvm::Value **outValue, CompilerContext *ctx)
{
    MAliceVisitFunction f = getNodeVisitFunction(node);

    return f(node, outValue, this, ctx);
}

bool ASTWalker::validateNode(ASTNode node, CompilerContext *ctx)
{
    MAliceValidationFunction f = getNodeValidationFunction(node);
    if (f == NULL)
    {
        return validateChildren(node, ctx);
    }
    else
    {
        return f(node, this, ctx);
    }
}
    
bool ASTWalker::visitChildren(ASTNode node, std::vector<llvm::Value*> *outValueList, CompilerContext *ctx)
{
    unsigned int numChildren = Utilities::getNumberOfChildNodes(node);
    bool result = true;
    
    std::vector<llvm::Value*> valueList;
    
    for (unsigned int i = 0; i < numChildren; ++i) {
        llvm::Value *value;
        if (!visitNode(Utilities::getChildNodeAtIndex(node, i), &value, ctx))
            result = false;
        
        valueList.push_back(value);
    }
    
    if (outValueList)
        *outValueList = valueList;
    
    return result;
}
    
bool ASTWalker::validateChildren(ASTNode node, CompilerContext *ctx)
{
    unsigned int numChildren = Utilities::getNumberOfChildNodes(node);
    bool result = true;
    
    for (unsigned int i = 0; i < numChildren; ++i) {
        if (!validateNode(Utilities::getChildNodeAtIndex(node, i), ctx))
            result = false;
    }
    
    return result;
}

MAliceVisitFunction ASTWalker::getNodeVisitFunction(ASTNode node)
{
    MAliceVisitFunction f = NULL;
    
    try {
        auto f = (visitDictionary.find(Utilities::getNodeType(node)));
        if (f == visitDictionary.end())
            return NULL;
        else
            return *f->second;

    }
    catch (std::out_of_range e) {
        return NULL;
    }
    
    return f;
}

MAliceValidationFunction ASTWalker::getNodeValidationFunction(ASTNode node)
{ 
    try {
        auto f = (validationDictionary.find(Utilities::getNodeType(node)));
        if (f == validationDictionary.end())
            return NULL;
        else
            return *f->second;
    }
    catch (std::out_of_range e) {
        return NULL;
    }
}


void ASTWalker :: compileTree()
{

}

}; // namespace MAlice