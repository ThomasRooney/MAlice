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
#include "CodeGeneration.h"

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
        
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(ARRAY, &CodeGeneration::generateCodeForArrayDeclarationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(ARRAYSUBSCRIPT, &CodeGeneration::generateCodeForArraySubscriptNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(ASSIGNMENTSTATEMENT, &CodeGeneration::generateCodeForAssignmentStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BANG, &CodeGeneration::generateCodeForLogicalNotExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BITWISEAND, &CodeGeneration::generateCodeForBitwiseAndExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BITWISEOR, &CodeGeneration::generateCodeForBitwiseOrExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BITWISEXOR, &CodeGeneration::generateCodeForBitwiseXorExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BLOCK, &CodeGeneration::generateCodeForArbitraryBlockNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BODY, &CodeGeneration::generateCodeForBodyNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BYREFERENCE, &CodeGeneration::generateCodeForByReferenceParameterNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(BYVALUE, &CodeGeneration::generateCodeForByValueParameterNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(CHARACTER_LITERAL, &CodeGeneration::generateCodeForCharacterLiteralNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(DECLS, &CodeGeneration::generateCodeForDeclarationsNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(DECREMENTSTATEMENT, &CodeGeneration::generateCodeForDecrementStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(DIVIDE, &CodeGeneration::generateCodeForDivideExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(EQUALS, &CodeGeneration::generateCodeForEqualsExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(EXPRESSION, &CodeGeneration::generateCodeForExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(FULL_STOP, &CodeGeneration::generateCodeForNullStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(FUNCDEFINITION, &CodeGeneration::generateCodeForFunctionDeclarationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(GREATERTHAN, &CodeGeneration::generateCodeForGreaterThanExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(GREATERTHANEQUAL, &CodeGeneration::generateCodeForGreaterThanOrEqualExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(IDENTIFIER, &CodeGeneration::generateCodeForIdentifierNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(IFSTATEMENT, &CodeGeneration::generateCodeForIfStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(INCREMENTSTATEMENT, &CodeGeneration::generateCodeForIncrementStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(INPUTSTATEMENT, &CodeGeneration::generateCodeForInputStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(INVOCATION, &CodeGeneration::generateCodeForProcFuncInvocationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LESSTHAN, &CodeGeneration::generateCodeForLessThanExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LESSTHANEQUAL, &CodeGeneration::generateCodeForLessThanOrEqualExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LETTER_TYPE, &CodeGeneration::generateCodeForLetterTypeNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LOGICALAND, &CodeGeneration::generateCodeForLogicalAndExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(LOGICALOR, &CodeGeneration::generateCodeForLogicalOrExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(MINUS, &CodeGeneration::generateCodeForMinusExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(MODULO, &CodeGeneration::generateCodeForModuloExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(MULTIPLY, &CodeGeneration::generateCodeForMultiplyExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(NOTEQUAL, &CodeGeneration::generateCodeForNotEqualExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(NUMBER_LITERAL, &CodeGeneration::generateCodeForNumberLiteralNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(NUMBER_TYPE, &CodeGeneration::generateCodeForNumberTypeNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PARAMS, &CodeGeneration::generateCodeForParamsNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PLUS, &CodeGeneration::generateCodeForPlusExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PRINTSTATEMENT, &CodeGeneration::generateCodeForPrintStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PROCDEFINITION, &CodeGeneration::generateCodeForProcedureDeclarationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(PROGRAM, &CodeGeneration::generateCodeForProgramNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(RETURNSTATEMENT, &CodeGeneration::generateCodeForReturnStatementNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(SENTENCE_TYPE, &CodeGeneration::generateCodeForSentenceTypeNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(STATEMENTLIST, &CodeGeneration::generateCodeForStatementListNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(STRING_LITERAL, &CodeGeneration::generateCodeForStringLiteralNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(TILDE, &CodeGeneration::generateCodeForLogicalNotExpressionNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(VARDECLARATION, &CodeGeneration::generateCodeForVariableDeclarationNode));
        ASTWalker::codeGenerationDictionary.insert(std::make_pair(WHILESTATEMENT, &CodeGeneration::generateCodeForWhileStatementNode));
   
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