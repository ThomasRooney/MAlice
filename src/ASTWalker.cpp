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
    constructCodeGenerationDictionary();
}
    
ASTWalker :: ~ASTWalker() {
}


// Initialise the static function dictionaries
std::unordered_map<unsigned int, MAliceCodeGenerationFunction> ASTWalker::codeGenerationDictionary = std::unordered_map<unsigned int, MAliceCodeGenerationFunction>();
std::unordered_map<unsigned int, MAliceValidationFunction> ASTWalker::validationDictionary = std::unordered_map<unsigned int, MAliceValidationFunction>();

void ASTWalker :: constructCodeGenerationDictionary() {
    static bool doOnce = false;
    if (!doOnce) {
        doOnce = true;
        
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
        
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(ARRAY, &generateCodeForArrayDeclarationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(ARRAYSUBSCRIPT, &generateCodeForArraySubscriptNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(ASSIGNMENTSTATEMENT, &generateCodeForAssignmentStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BANG, &generateCodeForLogicalNotExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BITWISEAND, &generateCodeForBitwiseAndExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BITWISEOR, &generateCodeForBitwiseOrExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BITWISEXOR, &generateCodeForBitwiseXorExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BLOCK, &generateCodeForArbitraryBlockNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BODY, &generateCodeForBodyNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BYREFERENCE, &generateCodeForByReferenceParameterNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BYVALUE, &generateCodeForByValueParameterNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(CHARACTER_LITERAL, &generateCodeForCharacterLiteralNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(DECLS, &generateCodeForDeclarationsNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(DECREMENTSTATEMENT, &generateCodeForDecrementStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(DIVIDE, &generateCodeForDivideExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(EQUALS, &generateCodeForEqualsExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(EXPRESSION, &generateCodeForExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(FULL_STOP, &generateCodeForNullStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(FUNCDEFINITION, &generateCodeForFunctionDeclarationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(GREATERTHAN, &generateCodeForGreaterThanExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(GREATERTHANEQUAL, &generateCodeForGreaterThanOrEqualExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(IDENTIFIER, &generateCodeForIdentifierNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(IFSTATEMENT, &generateCodeForIfStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(INCREMENTSTATEMENT, &generateCodeForIncrementStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(INPUTSTATEMENT, &generateCodeForInputStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(INVOCATION, &generateCodeForProcFuncInvocationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LESSTHAN, &generateCodeForLessThanExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LESSTHANEQUAL, &generateCodeForLessThanOrEqualExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LETTER_TYPE, &generateCodeForLetterTypeNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LOGICALAND, &generateCodeForLogicalAndExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LOGICALOR, &generateCodeForLogicalOrExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(MINUS, &generateCodeForMinusExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(MODULO, &generateCodeForModuloExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(MULTIPLY, &generateCodeForMultiplyExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(NOTEQUAL, &generateCodeForNotEqualExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(NUMBER_LITERAL, &generateCodeForNumberLiteralNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(NUMBER_TYPE, &generateCodeForNumberTypeNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PARAMS, &generateCodeForParamsNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PLUS, &generateCodeForPlusExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PRINTSTATEMENT, &generateCodeForPrintStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PROCDEFINITION, &generateCodeForProcedureDeclarationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PROGRAM, &generateCodeForProgramNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(RETURNSTATEMENT, &generateCodeForReturnStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(SENTENCE_TYPE, &generateCodeForSentenceTypeNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(STATEMENTLIST, &generateCodeForStatementListNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(STRING_LITERAL, &generateCodeForStringLiteralNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(TILDE, &generateCodeForLogicalNotExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(VARDECLARATION, &generateCodeForVariableDeclarationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(WHILESTATEMENT, &generateCodeForWhileStatementNode));
   
    }
}

bool ASTWalker::validateTree(pANTLR3_BASE_TREE root, CompilerContext *ctx)
{
  return validateNode(root, ctx);
}

bool ASTWalker::generateIRFromTree(pANTLR3_BASE_TREE root, CompilerContext *ctx)
{
    return generateCodeForNode(root, NULL, ctx);
}

bool ASTWalker::generateCodeForNode(ASTNode node, llvm::Value **outValue, CompilerContext *ctx)
{
    MAliceCodeGenerationFunction f = getNodeCodeGenerationFunction(node);

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
    
bool ASTWalker::generateCodeForChildren(ASTNode node, std::vector<llvm::Value*> *outValueList, CompilerContext *ctx)
{
    unsigned int numChildren = Utilities::getNumberOfChildNodes(node);
    bool result = true;
    
    std::vector<llvm::Value*> valueList;
    
    for (unsigned int i = 0; i < numChildren; ++i) {
        llvm::Value *value;
        if (!generateCodeForNode(Utilities::getChildNodeAtIndex(node, i), &value, ctx))
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

MAliceCodeGenerationFunction ASTWalker::getNodeCodeGenerationFunction(ASTNode node)
{
    MAliceCodeGenerationFunction f = NULL;
    
    try {
        auto f = (codeGenerationDictionary.find(Utilities::getNodeType(node)));
        if (f == codeGenerationDictionary.end())
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