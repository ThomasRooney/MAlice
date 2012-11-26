
#ifndef _MALICEUTILITIES
#define _MALICEUTILITIES

#include <iostream>
#include <unordered_map>

#include "Types.h"

namespace MAlice {
    
    class Entity;

    class Utilities {
    private:
        static pANTLR3_STRING _printTree(ASTNode node, int depth);
        
        static void createTokenIdentifierToTextMap();
    public:
        static unsigned int getNodeLineNumber(ASTNode node);
        static unsigned int getNodeColumnIndex(ASTNode node);
        
        static void printTree(ASTNode tree);
        
        static ANTLR3_UINT32 getNodeType(ASTNode node);
        static unsigned int getNumberOfChildNodes(ASTNode node);
        static ASTNode getChildNodeAtIndex(ASTNode node, unsigned int index);
        static char *getNodeText(ASTNode node);
        static MAliceType getTypeFromTypeString(std::string);
        static MAliceType getTypeFromNodeType(int nodeType);
        static const char* getNameOfTypeFromMAliceType(MAliceType);
        static MAliceEntityType getTypeOfEntity(Entity *entity);
        static std::string getTokenTextFromTokenIdentifier(ANTLR3_UINT32 tokenIdentifier);
    };
    
}; // namespace MAlice

#endif /* #ifndef _MALICEUTILITIES */
