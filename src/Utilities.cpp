
#include <stdexcept>

#include "Utilities.h"

#include "CompilerContext.h"
#include "Entity.h"
#include "VariableEntity.h"
#include "ProcedureEntity.h"
#include "FunctionEntity.h"
#include "VisitorCallbacks.h"
#include "ArrayEntity.h"

namespace MAlice {
    
    unsigned int Utilities::getNodeLineNumber(ASTNode node)
    {
        pANTLR3_COMMON_TOKEN token = node->getToken(node);
        return token->getLine(token);
    }
    
    unsigned int Utilities::getNodeColumnIndex(ASTNode node)
    {
        pANTLR3_COMMON_TOKEN token = node->getToken(node);
        return token->getCharPositionInLine(token);
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
    
    MAliceType Utilities::getTypeFromTypeString(std::string typeString)
    {
        if (!typeString.compare("number"))
            return MAliceTypeNumber;
        
        if (!typeString.compare("letter"))
            return MAliceTypeLetter;
        
        if (!typeString.compare("sentence"))
            return MAliceTypeSentence;
        
        return MAliceTypeUndefined;
    }

    MAliceType Utilities::getTypeFromNodeType(int nodeType) 
    {
        switch (nodeType)
        {
            case STRING_LITERAL:
                return MAliceTypeSentence;
            case CHARACTER_LITERAL:
                return MAliceTypeLetter;
            case NUMBER_LITERAL:
                return MAliceTypeNumber;
            default:
                return MAliceTypeUndefined;
        }        
    }

    const char* Utilities::getNameOfTypeFromMAliceType(MAliceType type)
    {
        int t = type;
        //TODO: Move me somewhere better (types.h no good as linker error)
        const char* MAliceTypeNames [] = {
            "Undefined Type",
            "Number Type",
            "Letter Type",
            "Sentence Type",
            "Boolean Type",
            };
        if (t < 0 || t >= 5)
          t = 0;
        return (MAliceTypeNames [t]);
    }

    
    MAliceEntityType Utilities::getTypeOfEntity(Entity *entity)
    {
        VariableEntity *variableEntity = dynamic_cast<VariableEntity*>(entity);
        if (variableEntity)
            return MAliceEntityTypeVariable;
        
        FunctionEntity *functionEntity = dynamic_cast<FunctionEntity*>(entity);
        if (functionEntity)
            return MAliceEntityTypeFunction;
        
        ProcedureEntity *procedureEntity = dynamic_cast<ProcedureEntity*>(entity);
        if (procedureEntity)
            return MAliceEntityTypeProcedure;
        
        ParameterEntity *parameterEntity = dynamic_cast<ParameterEntity*>(entity);
        if (parameterEntity)
            return MAliceEntityTypeParameter;
        
        ArrayEntity *arrayEntity = dynamic_cast<ArrayEntity*>(entity);
        if (arrayEntity)
            return MAliceEntityTypeArray;
        
        return MAliceEntityTypeUndefined;
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
    
}; // namespace MAlice