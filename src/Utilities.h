
#ifndef _MALICEUTILITIES
#define _MALICEUTILITIES

#include <iostream>

#include "Types.h"

namespace MAlice {

    class Utilities {
        
        static unsigned int getNodeLineNumber(ASTNode node);
        static unsigned int getNodeColumnIndex(ASTNode node);
        
    };
    
}; // namespace MAlice

#endif /* #ifndef _MALICEUTILITIES */
