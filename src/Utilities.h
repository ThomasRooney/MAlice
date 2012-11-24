
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
    };
    
}; // namespace MAlice

#endif /* #ifndef _MALICEUTILITIES */
