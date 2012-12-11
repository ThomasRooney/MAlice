
#include <stdexcept>
#include <ostream>
#include <sstream>
#include <list>
#include <iomanip>
#include "Utilities.h"
#include "CompilerContext.h"
#include "Entity.h"
#include "ErrorFactory.h"
#include "ErrorPosition.h"
#include "FunctionProcedureEntity.h"
#include "GlobalVariableEntity.h"
#include "VariableEntity.h"
#include "ProcedureEntity.h"
#include "FunctionEntity.h"
#include "VisitorCallbacks.h"
#include "Types.h"


namespace MAlice {
    
    unsigned int Utilities::getNodeLineNumber(ASTNode node)
    {
        node = getFirstNonImaginaryChildNode(node);
        
        pANTLR3_COMMON_TOKEN token = node->getToken(node);
        return token->getLine(token);
    }
    
    unsigned int Utilities::getNodeColumnIndex(ASTNode node)
    {
        node = getFirstNonImaginaryChildNode(node);
        
        pANTLR3_COMMON_TOKEN token = node->getToken(node);
        int charPos = token->getCharPositionInLine(token);
        return charPos<0?0:charPos;
    }
    
    void Utilities::printTree(ASTNode tree)
    {
        std::cout << _printTree(tree, 0)->chars;
    }
    
    pANTLR3_STRING Utilities::_printTree(ASTNode node, int depth)
    {
        pANTLR3_STRING string = node->strFactory->newRaw(node->strFactory);
        
        unsigned int numChildren = getNumberOfChildNodes(node);
        
        string->appendS	(string, node->toString(node));
        string->append8	(string, "\n");
        
        for (unsigned int childIndex = 0; childIndex < numChildren; ++childIndex)
        {
            for (int depthIndex = 0; depthIndex <= depth; ++depthIndex)
            {
                string->append8(string, "|");
                string->append8(string, " ");
            }

            string->chars[string->len-1] = '-';

            ASTNode child = getChildNodeAtIndex(node, childIndex);
            string->appendS(string, _printTree(child , depth+1));
        }
        
        return string;
    }
    
    static std::unordered_map<ANTLR3_UINT32, std::string> *tokenIdentifierToTextMap;
    
    ANTLR3_UINT32 Utilities::getNodeType(ASTNode node)
    {
        return node->getType(node);
    }
    
    unsigned int Utilities::getNumberOfChildNodes(ASTNode node)
    {
        pANTLR3_VECTOR children = node->children;
        if (children == NULL)
            return 0;
        
        return children->size(children);
    }
    
    ASTNode Utilities::getChildNodeAtIndex(ASTNode node, unsigned int index)
    {
        if (index >= Utilities::getNumberOfChildNodes(node))
            return NULL;
        
        pANTLR3_VECTOR children = node->children;
        return (ASTNode)children->get(children, index);
    }
    
    char * Utilities::getNodeText(ASTNode node)
    {
        return (char*)node->getText(node)->chars;
    }
    
    Type Utilities::getTypeFromTypeString(std::string typeString)
    {
        if (!typeString.compare("number"))
            return Type(PrimitiveTypeNumber);
        
        if (!typeString.compare("letter"))
            return Type(PrimitiveTypeLetter);
        
        if (!typeString.compare("sentence"))
            return Type(PrimitiveTypeSentence);
        
        return Type();
    }

    Type Utilities::getTypeFromNodeType(int nodeType)
    {
        switch (nodeType)
        {
            case STRING_LITERAL:
                return Type(PrimitiveTypeSentence);
            case CHARACTER_LITERAL:
                return Type(PrimitiveTypeLetter);
            case NUMBER_LITERAL:
                return Type(PrimitiveTypeNumber);
            default:
                return Type();
        }        
    }

    std::string Utilities::getNameOfPrimitiveType(Type type)
    {
        switch(type.getPrimitiveType())
        {
            case PrimitiveTypeNumber:
                return "number";
                break;
            case PrimitiveTypeLetter:
                return "letter";
                break;
            case PrimitiveTypeBoolean:
                return "boolean";
            case PrimitiveTypeSentence:
                return "sentence";
            default:
                return "undefined";
        }
    }

    MAliceEntityType Utilities::getTypeOfEntity(Entity *entity)
    {
        ParameterEntity *parameterEntity = dynamic_cast<ParameterEntity*>(entity);
        if (parameterEntity)
            return MAliceEntityTypeParameter;
        
        GlobalVariableEntity *globalVariableEntity = dynamic_cast<GlobalVariableEntity*>(entity);
        if (globalVariableEntity)
            return MAliceEntityTypeGlobalVariable;
        
        VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
        if (variableEntity)
            return MAliceEntityTypeVariable;
        
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(entity);
        if (functionEntity)
            return MAliceEntityTypeFunction;
        
        ProcedureEntity *procedureEntity = dynamic_cast<ProcedureEntity*>(entity);
        if (procedureEntity)
            return MAliceEntityTypeProcedure;
        
        return MAliceEntityTypeUndefined;
    }
    
    bool Utilities::isKindOfEntity(Entity *entity, MAliceEntityType type)
    {
        ParameterEntity *parameterEntity = dynamic_cast<ParameterEntity*>(entity);
        if (parameterEntity && (type & MAliceEntityTypeParameter) != 0)
            return true;
        
        GlobalVariableEntity *globalVariableEntity = dynamic_cast<GlobalVariableEntity*>(entity);
        if (globalVariableEntity && (type & MAliceEntityTypeGlobalVariable) != 0)
            return true;

        VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
        if (variableEntity && (type & MAliceEntityTypeVariable) != 0)
            return true;
        
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(entity);
        if (functionEntity && (type & MAliceEntityTypeFunction) != 0)
            return true;
        
        ProcedureEntity *procedureEntity = dynamic_cast<ProcedureEntity*>(entity);
        if (procedureEntity && (type & MAliceEntityTypeProcedure) != 0)
            return true;
        
        return false;
    }
    
    std::string Utilities::getTokenTextFromTokenIdentifier(ANTLR3_UINT32 tokenIdentifier)
    {
        if (!tokenIdentifierToTextMap)
            createTokenIdentifierToTextMap();
        
        std::unordered_map<ANTLR3_UINT32, std::string>::iterator el = tokenIdentifierToTextMap->find(tokenIdentifier);
        if (el != tokenIdentifierToTextMap->end())
            return el->second;
        
        return "";
    }
    
    // There doesn't appear to be a way to get the token text from a token identifier :(
    void Utilities::createTokenIdentifierToTextMap()
    {
        // We don't have to worry about cleanup, as we essentially think of the Utilities class as a singleton which
        // stays around for the entire program.
        tokenIdentifierToTextMap = new std::unordered_map<ANTLR3_UINT32, std::string>();
        
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(ALICEFOUND, "Alice found"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(ALICEWASUNSURE, "Alice was unsure which"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(ALICEFOUND, "Alice found"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(AND, "and"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(APOSTROPHE_S, "'s"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(ATE, "ate"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(BANG, "!"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(BECAME, "became"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(BECAUSE, "because"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(BITWISEAND, "&"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(BITWISEOR, "|"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(BITWISEXOR, "^"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(BUT, "but"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(CLOSED, "closed"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(COMMA, ","));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(CONTAINEDA, "contained a"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(DIVIDE, "/"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(DRANK, "drank"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(EITHER, "either"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(ENOUGHTIMES, "enough times"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(EQUALS, "=="));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(EVENTUALLY, "eventually"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(FULL_STOP, "."));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(GREATERTHAN, ">"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(GREATERTHANEQUAL, ">="));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(HAD, "had"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(LESSTHAN, "<"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(LESSTHANEQUAL, "<="));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(LETTER_TYPE, "letter"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(LOGICALAND, "&&"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(LOGICALOR, "||"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(LPAREN, "("));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(MAYBE, "maybe"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(MINUS, "-"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(MODULO, "%"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(MULTIPLY, "*"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(NOTEQUAL, "!="));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(NUMBER_TYPE, "number"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(OF, "of"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(OPENED, "opened"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(OR, "or"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(PERHAPS, "perhaps"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(PIECE, "piece"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(PLUS, "+"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(QUESTION_MARK, "?"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(QUOTE, "\""));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(RPAREN, ")"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(SAIDALICE, "said Alice"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(SO, "so"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(SPIDER, "spider"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(SPOKE, "spoke"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(THELOOKINGGLASS, "The looking-glass"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(THEN, "then"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(THEROOM, "The room"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(TILDE, "~"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(TOO, "too"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(UNDERSCORE, "_"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(WASA, "was a"));
        tokenIdentifierToTextMap->insert(std::pair<ANTLR3_UINT32, std::string>(WHATWAS, "what was"));
    }
    
    std::string Utilities::getNodeTextIncludingChildren(ASTNode node, CompilerContext *ctx, Range **outRange)
    {
        pANTLR3_COMMON_TREE superNode = (pANTLR3_COMMON_TREE)node->super;
        
        pANTLR3_COMMON_TOKEN_STREAM tokenStream = ctx->getTokenStream();
        pANTLR3_VECTOR tokens = tokenStream->getTokens(tokenStream);
        pANTLR3_COMMON_TOKEN startToken = (pANTLR3_COMMON_TOKEN)tokens->get(tokens, (ANTLR3_UINT32)superNode->startIndex);
        pANTLR3_COMMON_TOKEN endToken = (pANTLR3_COMMON_TOKEN)tokens->get(tokens, (ANTLR3_UINT32)superNode->stopIndex);
        
        if (outRange) {
            std::string endTokenString = (char*)endToken->input->substr(endToken->input, endToken->start, endToken->stop)->chars;
            
            *outRange = Utilities::createRange(startToken->line, startToken->charPosition,
                                               endToken->line, endToken->charPosition + (unsigned int)endTokenString.size() - 1);
        }
        
        return (char*)startToken->input->substr(startToken->input, startToken->start, endToken->stop)->chars;
    }
    
    ASTNode Utilities::getLeftDeepestChildNode(ASTNode node)
    {
        unsigned int numChildren = Utilities::getNumberOfChildNodes(node);
        
        if (numChildren == 0)
            return node;
        
        return getLeftDeepestChildNode(Utilities::getChildNodeAtIndex(node, 0));
    }
    
    ASTNode Utilities::getRightDeepestChildNode(ASTNode node)
    {
        unsigned int numChildren = Utilities::getNumberOfChildNodes(node);
        
        if (numChildren == 0)
            return node;
        
        return getLeftDeepestChildNode(Utilities::getChildNodeAtIndex(node, numChildren - 1));
    }
    
    std::string Utilities::stringWithLineIndentation(std::string string, unsigned int lineIndentation)
    {
        std::istringstream stringStream(string);
        std::string line;
        std::stringstream output;
        
        while (std::getline(stringStream, line)) {
            output << std::setw(2) << line << "\n";
        }
        
        return output.str();
    }
    
    bool Utilities::isImaginaryNode(ASTNode node)
    {
        pANTLR3_COMMON_TOKEN token = node->getToken(node);
        if (!token)
            return true;
        
        return token->start == 0 && token->stop == 0;
    }
    
    ASTNode Utilities::getFirstNonImaginaryChildNode(ASTNode node)
    {
        if (getNumberOfChildNodes(node) == 0 || !isImaginaryNode(node))
            return node;
        
        return getFirstNonImaginaryChildNode(getChildNodeAtIndex(node, 0));
    }

    std::list<Range*> Utilities::rangeToSingletonList(Range *r)
    {
        std::list<Range*> singleton = std::list<Range*>();
        singleton.push_back(r);
        return singleton;
    }
    
    Range *Utilities::createRange(unsigned int startLine, unsigned int startColumn)
    {
        return createRange(startLine, startColumn, startLine, startColumn);
    }
    
    Range *Utilities::createRange(unsigned int startLine, unsigned int startColumn, unsigned int endColumn)
    {
        return createRange(startLine, startColumn, startLine, endColumn);
    }
    
    Range *Utilities::createRange(unsigned int startLine, unsigned int startColumn, unsigned int endLine, unsigned int endColumn)
    {
        return new Range(ErrorPosition(startLine, startColumn), ErrorPosition(endLine, endColumn));
    }
    
    std::string Utilities::stripLeadingAndTrailingCharacters(std::string input, char character)
    {
        unsigned int startIndex = 0;
        unsigned int endIndex = (unsigned int)input.size() - 1;
        
        if (input[0] == character)
            startIndex++;
        
        if (input[input.size() - 1] == character)
            endIndex--;
        
        return input.substr(startIndex, endIndex - startIndex + 1);
    }
    
    std::string Utilities::getOperatorStringFromOperatorNode(ASTNode node)
    {
        switch(Utilities::getNodeType(node))
        {
            case PLUS:
                return "+";
                break;
            case MINUS:
                return "-";
                break;
            case EQUALS:
                return "==";
                break;
            case NOTEQUAL:
                return "!=";
                break;
            case LESSTHAN:
                return "<";
                break;
            case LESSTHANEQUAL:
                return "<=";
                break;
            case GREATERTHAN:
                return ">";
                break;
            case GREATERTHANEQUAL:
                return ">=";
                break;
            case TILDE:
                return "~";
                break;
            case BANG:
                return "!";
                break;
            case BITWISEAND:
                return "&";
                break;
            case BITWISEOR:
                return "|";
                break;
            case BITWISEXOR:
                return "^";
                break;
            case MODULO:
                return "%";
                break;
            case MULTIPLY:
                return "*";
                break;
            case DIVIDE:
                return "/";
            case LOGICALAND:
                return "&&";
                break;
            case LOGICALOR:
                return "||";
                break;
            default:
                return "";
                break;
        }
    }
    
    std::string Utilities::getTypeListFromTypes(std::vector<Type> types)
    {
        std::list<std::string> typeStrings;
        
        for (auto it = types.begin(); it != types.end(); ++it) {
            typeStrings.push_back(getNameOfPrimitiveType(*it));
        }
        
        std::string listString;
        unsigned int i = 0;
        
        for (std::list<std::string>::iterator it = typeStrings.begin(); it != typeStrings.end(); ++it) {
            if (it != typeStrings.begin()) {
                if (i == typeStrings.size() - 1)
                    listString.append(" or ");
                else
                    listString.append(", ");
            }
            
            listString.append("'");
            listString.append(*it);
            listString.append("'");
            
            ++i;
        }
        
        return listString;
    }
    
    void Utilities::printSymbolTableEntries(SymbolTable *symbolTable)
    {
        std::vector<std::string> identifiers = symbolTable->getAllIdentifiers();
        
        for (std::vector<std::string>::iterator it = identifiers.begin(); it != identifiers.end(); ++it) {
            std::cout << *it << ": " << symbolTable->get(*it) << "\n";
        }
    }
    
    std::vector<ParameterEntity*> Utilities::getParameterTypesFromParamsNode(ASTNode paramsNode)
    {
        std::vector<ParameterEntity*> parameterTypes;
        
        for (unsigned int i = 0; i < Utilities::getNumberOfChildNodes(paramsNode); ++i) {
            ASTNode childNode = Utilities::getChildNodeAtIndex(paramsNode, i);
            bool passedByReference = (Utilities::getNodeType(childNode) == BYREFERENCE);
            
            ASTNode passTypeNode = Utilities::getChildNodeAtIndex(childNode, 1);
            std::string identifier = Utilities::getNodeText(passTypeNode);
            int lineNumber = Utilities::getNodeLineNumber(passTypeNode);
            ASTNode typeNode = Utilities::getChildNodeAtIndex(childNode, 0);
            
            std::string typeString = Utilities::getNodeText(typeNode);
            Type t = Utilities::getTypeFromTypeString(typeString);
            if (passedByReference)
                t.setIsArray(true);
            
            ParameterEntity *p = new ParameterEntity(identifier, lineNumber, t);
            
            parameterTypes.push_back(p);
        }
        
        return parameterTypes;
    }
    
    FunctionProcedureEntity *Utilities::getFunctionProcedureEntityForInvocationNode(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        std::string identifier = getFunctionProcedureInvocationIdentifier(invocationNode, walker, ctx);
        Entity *entity = NULL;
        
        if (!ctx->isSymbolInScope(identifier, &entity))
            return NULL;
        
        MAliceEntityType entityType = Utilities::getTypeOfEntity(entity);
        
        if (entityType != MAliceEntityTypeProcedure && entityType != MAliceEntityTypeFunction)
            return NULL;
        
        return dynamic_cast<FunctionProcedureEntity*>(entity);
    }
    
    Type Utilities::getReturnTypeForInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = getFunctionProcedureEntityForInvocationNode(invocationNode, walker, ctx);
        if (!funcProcEntity) {
            return Type::voidType();
        }
        
        MAliceEntityType entityType = Utilities::getTypeOfEntity(funcProcEntity);
        
        if (entityType == MAliceEntityTypeProcedure)
            return Type::voidType();
        
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(funcProcEntity);
        
        return functionEntity->getReturnType();
    }
    
    std::string Utilities::getFunctionProcedureInvocationIdentifier(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx)
    {
        if (Utilities::getNumberOfChildNodes(invocationNode) == 0) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("function or procedure invocation"));
            return NULL;
        }
        
        ASTNode identifierNode = Utilities::getChildNodeAtIndex(invocationNode, 0);
        return Utilities::getNodeText(identifierNode);
    }
    
    bool Utilities::getTypeFromInvocationExpressionNode(ASTNode node, Type *outType, ASTWalker *walker, CompilerContext *ctx)
    {
        FunctionProcedureEntity *funcProcEntity = Utilities::getFunctionProcedureEntityForInvocationNode(node, walker, ctx);
        if (!funcProcEntity)
            return false;
        
        MAliceEntityType funcProcType = Utilities::getTypeOfEntity(funcProcEntity);
        if (funcProcType != MAliceEntityTypeFunction && funcProcType != MAliceEntityTypeProcedure) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("identifier in expression"));
            return false;
        }
        
        if (funcProcType == MAliceEntityTypeProcedure) {
            Range *procCallRange = NULL;
            Utilities::getNodeTextIncludingChildren(node, ctx, &procCallRange);
            
            Error *error = ErrorFactory::createSemanticError("Cannot use void result of procedure '" +
                                                             funcProcEntity->getIdentifier() +
                                                             "' in expression.");
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(procCallRange));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        Type returnType = Utilities::getReturnTypeForInvocation(node, walker, ctx);
        if (outType)
            *outType = returnType;
        
        return true;
    }
    
    bool Utilities::getTypeFromExpressionNode(ASTNode node, Type *outType, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, bool *passedByReference)
    {
        ASTNode firstChildNode = Utilities::getChildNodeAtIndex(node, 0);
        if (!firstChildNode) {
            // Possibly the firstChildNode has been passed in directly to this function. For example in the case of 'ate'/'drank'
            firstChildNode = node;
        }
        
        // Check identifiers first, because on their own they can reference an array if we don't want an l-value
        if (!requiresLValue && Utilities::getNodeType(firstChildNode) == IDENTIFIER) {
            Type type = Type::voidType();
            MAliceEntityType entityType = MAliceEntityTypeUndefined;
            if (!getTypeFromExpressionIdentifierNode(firstChildNode, &type, &entityType, walker, ctx, passedByReference))
                return false;
            
            if (outType)
                *outType = type;
            
            return true;
        }
        
        return getTypeFromExpressionRuleNode(firstChildNode, outType, walker, ctx);
    }
    
    bool Utilities::getTypeFromExpressionRuleNode(ASTNode node, Type *outType, ASTWalker *walker, CompilerContext *ctx)
    {
        switch (Utilities::getNodeType(node))
        {
            case IDENTIFIER:
            {
                Type type = Type::voidType();
                MAliceEntityType entityType = MAliceEntityTypeUndefined;
                
                if (!getTypeFromExpressionIdentifierNode(node, &type, &entityType, walker, ctx, NULL))
                    return false;
                
                if (entityType == MAliceEntityTypeArray) {
                    // Array has no children, hence this is an error!
                    Error *error = ErrorFactory::createSemanticError("Cannot refer to array '" + std::string(Utilities::getNodeText(node)) + "' directly.");
                    error->setLineNumber(Utilities::getNodeLineNumber(node));
                    error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node))));
                    error->setAdditionalInformation("To fix, specify an array element.");
                    ctx->getErrorReporter()->reportError(error);
                }
                
                if (outType)
                    *outType = type;
                
                return true;
            }
                break;
            case INVOCATION:
                return Utilities::getTypeFromInvocationExpressionNode(node, outType, walker, ctx);
                break;
            case EQUALS:
            {
                std::vector<Type> types;
                types.push_back(Type(PrimitiveTypeNumber));
                types.push_back(Type(PrimitiveTypeLetter));
                types.push_back(Type(PrimitiveTypeSentence));
                
                if (!getTypeFromBinaryOperatorNode(node, NULL, "equality", types, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case NOTEQUAL:
            {
                std::vector<Type> types;
                types.push_back(Type(PrimitiveTypeNumber));
                types.push_back(Type(PrimitiveTypeLetter));
                types.push_back(Type(PrimitiveTypeSentence));
                
                if (!getTypeFromBinaryOperatorNode(node, NULL, "inequality", types, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case LESSTHAN:
            {
                std::vector<Type> types;
                types.push_back(Type(PrimitiveTypeNumber));
                types.push_back(Type(PrimitiveTypeLetter));
                
                if (!getTypeFromBinaryOperatorNode(node, NULL, "less than", types, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case LESSTHANEQUAL:
            {
                std::vector<Type> types;
                types.push_back(Type(PrimitiveTypeNumber));
                types.push_back(Type(PrimitiveTypeLetter));
                
                if (!getTypeFromBinaryOperatorNode(node, NULL, "less than or equal", types, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case GREATERTHAN:
            {
                std::vector<Type> types;
                types.push_back(Type(PrimitiveTypeNumber));
                types.push_back(Type(PrimitiveTypeLetter));
                
                if (!getTypeFromBinaryOperatorNode(node, NULL, "greater than", types, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case GREATERTHANEQUAL:
            {
                std::vector<Type> types;
                types.push_back(Type(PrimitiveTypeNumber));
                types.push_back(Type(PrimitiveTypeLetter));
                
                if (!getTypeFromBinaryOperatorNode(node, NULL, "greater than or equal", types, walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case ARRAYSUBSCRIPT:
            {
                ASTNode identifierNode = Utilities::getChildNodeAtIndex(node, 0);
                if (!identifierNode || Utilities::getNodeType(identifierNode) != IDENTIFIER) {
                    ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError("array subscript"));
                    return false;
                }
                
                std::string identifier = Utilities::getNodeText(identifierNode);
                
                MAliceEntityType entityType = MAliceEntityTypeUndefined;
                Type type = Type::voidType();
                if (!getTypeFromExpressionIdentifierNode(identifierNode, &type, &entityType, walker, ctx, NULL))
                    return false;
                
                if (!type.isArray()) {
                    Range *identifierRange = NULL;
                    Utilities::getNodeTextIncludingChildren(node, ctx, &identifierRange);
                    
                    Error *error = ErrorFactory::createSemanticError("Invalid array subscript: '" + identifier + "' does not reference an array.");
                    error->setLineNumber(Utilities::getNodeLineNumber(identifierNode));
                    error->setUnderlineRanges(Utilities::rangeToSingletonList(identifierRange));
                    
                    ctx->getErrorReporter()->reportError(error);
                    
                    return false;
                }
                
                Entity *entity = NULL;
                ctx->isSymbolInScope(identifier, &entity);
                
                VariableEntity *arrayEntity = dynamic_cast<VariableEntity*>(entity);
                if (!arrayEntity) {
                    ctx->getErrorReporter()->reportError(ErrorFactory::createInternalError("Could not get valid entity from symbol table for array '" + identifier + "'."));
                    return false;
                }
                
                Type subscriptType = Type::voidType();
                ASTNode subscriptExpressionNode = Utilities::getChildNodeAtIndex(node, 1);
                if (!getTypeFromExpressionNode(subscriptExpressionNode, &subscriptType, false, walker, ctx, NULL))
                    return false;
                
                if (subscriptType.getPrimitiveType() != PrimitiveTypeNumber) {
                    ctx->getErrorReporter()->reportError(ErrorFactory::createCannotMatchTypesError(subscriptExpressionNode,
                                                                                                   Type(PrimitiveTypeNumber),
                                                                                                   subscriptType,
                                                                                                   ctx));
                    return false;
                }
                
                
                if (outType)
                    *outType = arrayEntity->getType();
                
                return true;
            }
                break;
            case PLUS:
            {
                if (Utilities::getNumberOfChildNodes(node) == 2) {
                    Type type = Type::voidType();
                    std::vector<Type> types;
                    types.push_back(Type(PrimitiveTypeNumber));
                    types.push_back(Type(PrimitiveTypeSentence));
                    types.push_back(Type(PrimitiveTypeLetter));
                    
                    if (!getTypeFromBinaryOperatorNode(node, &type, "addition", types, walker, ctx))
                        return false;
                    
                    if (outType)
                        *outType = type;
                } else {
                    if (!getTypeFromUnaryOperatorNode(node, NULL, "unary plus", Type(PrimitiveTypeNumber), walker, ctx))
                        return false;
                    
                    if (outType)
                        *outType = Type(PrimitiveTypeNumber);
                }
            }
                break;
            case MINUS:
            {
                if (Utilities::getNumberOfChildNodes(node) == 2) {
                    Type type = Type::voidType();
                    std::vector<Type> types;
                    types.push_back(Type(PrimitiveTypeNumber));
                    types.push_back(Type(PrimitiveTypeLetter));
                    types.push_back(Type(PrimitiveTypeSentence));
                    
                    if (!getTypeFromBinaryOperatorNode(node, &type, "subtraction", types, walker, ctx))
                        return false;
                    
                    if (outType)
                        *outType = type;
                } else {
                    if (!getTypeFromUnaryOperatorNode(node, NULL, "unary minus", Type(PrimitiveTypeNumber), walker, ctx))
                        return false;
                    
                    if (outType)
                        *outType = Type(PrimitiveTypeNumber);
                }
            }
                break;
            case TILDE:
            {
                if (!getTypeFromUnaryOperatorNode(node, NULL, "tilde", Type(PrimitiveTypeNumber), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeNumber);
            }
                break;
            case BANG:
            {
                if (!getTypeFromUnaryOperatorNode(node, NULL, "unary NOT", Type(PrimitiveTypeBoolean), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case BITWISEAND:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "bitwise AND", singletonVector(Type(PrimitiveTypeNumber)), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeNumber);
            }
                break;
            case BITWISEOR:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "bitwise OR", singletonVector(Type(PrimitiveTypeNumber)), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeNumber);
            }
                break;
            case BITWISEXOR:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "bitwise XOR", singletonVector(Type(PrimitiveTypeNumber)), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeNumber);
            }
                break;
            case MODULO:
            {
                Type type = Type::voidType();
                if (!getTypeFromBinaryOperatorNode(node, &type, "modulo", singletonVector(Type(PrimitiveTypeNumber)), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = type;
            }
                break;
            case MULTIPLY:
            {
                Type type = Type::voidType();
                if (!getTypeFromBinaryOperatorNode(node, &type, "multiplication", singletonVector(Type(PrimitiveTypeNumber)), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = type;
            }
                break;
            case DIVIDE:
            {
                Type type = Type::voidType();
                if (!getTypeFromBinaryOperatorNode(node, &type, "division", singletonVector(Type(PrimitiveTypeNumber)), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = type;
            }
                break;
            case LOGICALAND:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "logical and", singletonVector(Type(PrimitiveTypeBoolean)), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case LOGICALOR:
            {
                if (!getTypeFromBinaryOperatorNode(node, NULL, "logical or", singletonVector(Type(PrimitiveTypeBoolean)), walker, ctx))
                    return false;
                
                if (outType)
                    *outType = Type(PrimitiveTypeBoolean);
            }
                break;
            case NUMBER_LITERAL:
                if (outType)
                    *outType = Type(PrimitiveTypeNumber);
                break;
            case CHARACTER_LITERAL:
                if (outType)
                    *outType = Type(PrimitiveTypeLetter);
                break;
            case STRING_LITERAL:
                if (outType)
                    *outType = Type(PrimitiveTypeSentence);
                break;
            default:
                break;
        }
        
        return true;
    }
    
    bool Utilities::getTypeFromBinaryOperatorNode(ASTNode node, Type *outType, std::string operatorName, std::vector<Type> requiredTypes, ASTWalker *walker, CompilerContext *ctx)
    {
        Type t1, t2;
        unsigned int numChildNodes = Utilities::getNumberOfChildNodes(node);
        
        if (numChildNodes != 2) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError(operatorName +
                                                                                     " operator in expression " +
                                                                                     Utilities::getOperatorStringFromOperatorNode(node) +
                                                                                     "."));
            return false;
        }
        
        ASTNode leftOperandRootNode = Utilities::getChildNodeAtIndex(node, 0);
        ASTNode rightOperandRootNode = Utilities::getChildNodeAtIndex(node, 1);
        
        if (!getTypeFromExpressionRuleNode(leftOperandRootNode, &t1, walker, ctx))
            return false;
        
        if (!getTypeFromExpressionRuleNode(rightOperandRootNode, &t2, walker, ctx))
            return false;
        
        ;
        
        if (t1 != t2) {
            Error *error = ErrorFactory::createSemanticError("Cannot match types '" +
                                                             std::string(Utilities::getNameOfPrimitiveType(t1)) +
                                                             "' and '" +
                                                             std::string(Utilities::getNameOfPrimitiveType(t2)) +
                                                             "' for operands to binary operator.");
            
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            
            Range *leftOperandRange = NULL, *rightOperandRange = NULL;
            Utilities::getNodeTextIncludingChildren(leftOperandRootNode, ctx, &leftOperandRange);
            Utilities::getNodeTextIncludingChildren(rightOperandRootNode, ctx, &rightOperandRange);
            
            std::list<Range*> underlineRanges;
            underlineRanges.push_back(leftOperandRange);
            underlineRanges.push_back(rightOperandRange);
            
            error->setUnderlineRanges(underlineRanges);
            
            unsigned int operatorStringLength = (unsigned int)Utilities::getOperatorStringFromOperatorNode(node).size();
            Range *arrowRange = Utilities::createRange(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node),
                                                       Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node) + operatorStringLength - 1);
            
            error->setArrowRanges(Utilities::rangeToSingletonList(arrowRange));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        if (std::find(requiredTypes.begin(), requiredTypes.end(), t1) == requiredTypes.end()) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidOperandTypeError(node, leftOperandRootNode, t1, requiredTypes, ctx));
            
            return false;
        }
        
        if (std::find(requiredTypes.begin(), requiredTypes.end(), t2) == requiredTypes.end()) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidOperandTypeError(node, rightOperandRootNode, t2, requiredTypes, ctx));
            
            return false;
        }
        
        if (outType)
            *outType = t1;
        
        return true;
    }
    
    bool Utilities::getTypeFromUnaryOperatorNode(ASTNode node, Type *outType, std::string operatorName, Type requiredType, ASTWalker *walker, CompilerContext *ctx)
    {
        Type type = Type::voidType();
        unsigned int numChildNodes = Utilities::getNumberOfChildNodes(node);
        
        if (numChildNodes != 1) {
            ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidASTError(operatorName + " operator in expression"));
            return false;
        }
        
        ASTNode operandRootNode = Utilities::getChildNodeAtIndex(node, 0);
        
        if (!getTypeFromExpressionRuleNode(operandRootNode, &type, walker, ctx))
            return false;
        
        if (requiredType != Type::voidType()) {
            if (type != requiredType) {
                ctx->getErrorReporter()->reportError(ErrorFactory::createInvalidOperandTypeError(operandRootNode, requiredType, type, ctx));
                
                return false;
            }
        }
        
        if (outType)
            *outType = type;
        
        return true;
    }
    
    bool Utilities::getTypeFromExpressionIdentifierNode(ASTNode node, Type *outType, MAliceEntityType *outEntityType, ASTWalker *walker, CompilerContext *ctx, bool *passedByReference)
    {
        Entity *lookupEntity;
        
        std::string identifier = Utilities::getNodeText(node);
        
        if (!ctx->isSymbolInScope(identifier, &lookupEntity)) {
            Range *range = NULL;
            
            Utilities::getNodeTextIncludingChildren(node, ctx, &range);
            
            Error *error = ErrorFactory::createSemanticError("Identifier: '" + identifier + "' is not in scope.");
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setUnderlineRanges(Utilities::rangeToSingletonList(range));
            
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        VariableEntity *lookupVEntity = dynamic_cast<VariableEntity*>(lookupEntity);
        if (!lookupVEntity) {
            Error *error = ErrorFactory::createSemanticError("Identifier '" + identifier + "' does not have a type.");
            error->setLineNumber(Utilities::getNodeLineNumber(node));
            error->setArrowRanges(Utilities::rangeToSingletonList(Utilities::createRange(Utilities::getNodeLineNumber(node), Utilities::getNodeColumnIndex(node))));
            ctx->getErrorReporter()->reportError(error);
            
            return false;
        }
        
        if (outType)
        {
            *outType = lookupVEntity->getType();
            if (passedByReference)
                *passedByReference = lookupVEntity->getType().isArray();
        }
        
        if (outEntityType)
            *outEntityType = Utilities::getTypeOfEntity(lookupVEntity);
        
        return true;
    }
    
    llvm::Type *Utilities::getLLVMTypeFromType(Type type)
    {
        const llvm::Type *rType = NULL; 

        switch(type.getPrimitiveType())
        {
            case PrimitiveTypeNumber:
                rType = llvm::Type::getInt64Ty(llvm::getGlobalContext());
                break;
            case PrimitiveTypeLetter:
                rType = llvm::Type::getInt64Ty(llvm::getGlobalContext());
                break;
            case PrimitiveTypeBoolean:
                rType = llvm::Type::getInt1Ty(llvm::getGlobalContext());
                break;
            case PrimitiveTypeSentence:
                rType = llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
                break;
            default:
                rType = llvm::Type::getVoidTy(llvm::getGlobalContext());
                break;
        }
        return const_cast<llvm::Type *>(rType);
    }

}; // namespace MAlice