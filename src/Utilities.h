
#ifndef _MALICEUTILITIES
#define _MALICEUTILITIES

#include <iostream>

#include "Types.h"

namespace MAlice {

    class Utilities {
    private:
        static pANTLR3_STRING _printTree(ASTNode node, int depth);
    public:
        static unsigned int getNodeLineNumber(ASTNode node);
        static unsigned int getNodeColumnIndex(ASTNode node);
        
        static void printTree(ASTNode tree);
        
        static ANTLR3_UINT32 getNodeType(ASTNode node);
        static unsigned int getNumberOfChildNodes(ASTNode node);
        static ASTNode getChildNodeAtIndex(ASTNode node, unsigned int index);
        static char *getNodeText(ASTNode node);
        static MAliceType getTypeFromTypeString(std::string);
        static bool confirmTypeOfExpression(ASTNode node, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm);
        static MAliceType getTypeFromNodeType(int nodeType);
        static char* getNameOfTypeFromMAliceType(MAliceType);
    };
    
}; // namespace MAlice

#endif /* #ifndef _MALICEUTILITIES */
