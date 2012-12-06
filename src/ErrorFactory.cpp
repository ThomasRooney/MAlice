
#include "ErrorFactory.h"

#include "CompilerContext.h"
#include "Error.h"
#include "Range.h"
#include "Utilities.h"

namespace MAlice {
    
    Error *ErrorFactory::createWarningError(std::string errorMessage)
    {
        return new Error(ErrorType::Warning, errorMessage);
    }
    
    Error *ErrorFactory::createInternalError(std::string errorMessage)
    {
        return new Error(ErrorType::Internal, errorMessage);
    }

    Error *ErrorFactory::createLexicalError()
    {
        return new Error(ErrorType::Lexical);
    }
    
    Error *ErrorFactory::createLexicalError(std::string errorMessage)
    {
        return new Error(ErrorType::Lexical, errorMessage);
    }
    
    Error *ErrorFactory::createSyntacticError(std::string errorMessage)
    {
        return new Error(ErrorType::Syntactic, errorMessage);
    }
    
    Error *ErrorFactory::createSemanticError(std::string errorMessage)
    {
        return new Error(ErrorType::Semantic, errorMessage);
    }
    
    Error *ErrorFactory::createIOError(std::string errorMessage)
    {
        return new Error(ErrorType::IO, errorMessage);
    }
  
    Error *ErrorFactory::createInvalidASTError(std::string currentValidationType)
    {
        return createInternalError("Incorrect internal AST representation encountered (from validation of " + currentValidationType + ").");
    }
    
    Error *ErrorFactory::createInvalidLValueError(ASTNode node, CompilerContext *ctx)
    {
        Range *errorRange = NULL;
        std::string invalidExpression = Utilities::getNodeTextIncludingChildren(node, ctx, &errorRange);
        
        Error *error = createSemanticError("'" + invalidExpression + "' is not an l-value.");
        error->setLineNumber(Utilities::getNodeLineNumber(node));
        error->setUnderlineRanges(Utilities::rangeToSingletonList(errorRange));
        
        return error;
    }
    
    Error *ErrorFactory::createInvalidOperandTypeError(ASTNode operandNode, MAliceType expectedType, MAliceType actualType, CompilerContext *ctx)
    {
        Range *errorRange = NULL;
        std::string operandString = Utilities::getNodeTextIncludingChildren(operandNode, ctx, &errorRange);
        
        Error *error = ErrorFactory::createSemanticError("Cannot match type '" +
                                                         std::string(Utilities::getNameOfTypeFromMAliceType(actualType)) +
                                                         "' with expected type '" +
                                                         std::string(Utilities::getNameOfTypeFromMAliceType(expectedType)) +
                                                         "' in operand '" +
                                                         operandString +
                                                         "'.");
        error->setLineNumber(Utilities::getNodeLineNumber(operandNode));
        error->setUnderlineRanges(Utilities::rangeToSingletonList(errorRange));
        
        return error;
    }
    
    Error *ErrorFactory::createInvalidOperandTypeError(ASTNode operatorNode, ASTNode operandNode, MAliceType actualType, unsigned int expectedTypes, CompilerContext *ctx)
    {
        Range *errorRange = NULL;
        std::string operandString = Utilities::getNodeTextIncludingChildren(operandNode, ctx, &errorRange);
        
        Error *error = ErrorFactory::createSemanticError("Cannot match type '" +
                                                         std::string(Utilities::getNameOfTypeFromMAliceType(actualType)) +
                                                         "' with expected types " +
                                                         Utilities::getTypeListFromTypeFlags(expectedTypes) +
                                                         " for operand '" +
                                                         operandString +
                                                         "' over operator "
                                                         + Utilities::getOperatorStringFromOperatorNode(operatorNode) +
                                                         ".");
        unsigned int lineNumber = Utilities::getNodeLineNumber(operatorNode);
        unsigned int columnIndex = Utilities::getNodeColumnIndex(operatorNode);
        unsigned int arrowUnderlineLength = (unsigned int)Utilities::getOperatorStringFromOperatorNode(operatorNode).size() - 1;
        
        error->setLineNumber(Utilities::getNodeLineNumber(operandNode));
        error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(lineNumber, columnIndex, columnIndex + arrowUnderlineLength)));
        error->setUnderlineRanges(Utilities::rangeToSingletonList(errorRange));
        
        return error;
    }
    
    Error *ErrorFactory::createCannotMatchTypesError(ASTNode exprNode, MAliceType expectedType, MAliceType actualType, CompilerContext *ctx)
    {
        Range *exprRange = NULL;
        std::string exprText = Utilities::getNodeTextIncludingChildren(exprNode, ctx, &exprRange);
        
        ASTNode firstNonImaginaryNode = Utilities::getFirstNonImaginaryChildNode(exprNode);
        Error *error = ErrorFactory::createSemanticError("Can't match expected type '" +
                                                         std::string(Utilities::getNameOfTypeFromMAliceType(expectedType)) +
                                                         "' with type '" +
                                                         std::string(Utilities::getNameOfTypeFromMAliceType(actualType)) +
                                                         "' for expression '" +
                                                         Utilities::stripLeadingAndTrailingCharacters(exprText, '\'') + "'.");
        error->setLineNumber(Utilities::getNodeLineNumber(firstNonImaginaryNode));
        error->setUnderlineRanges(Utilities::rangeToSingletonList(exprRange));
        
        return error;
    }
    
};