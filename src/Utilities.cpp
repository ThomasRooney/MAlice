
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
    
}; // namespace MAlice