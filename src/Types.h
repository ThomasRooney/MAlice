//
//  Types.h
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#ifndef _MALICETYPES
#define _MALICETYPES

#include "MAliceParser.h"

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;

    typedef pANTLR3_BASE_TREE ASTNode;
    typedef bool (*MAliceVisitFunction)(ASTNode, ASTWalker*, CompilerContext*);
    
    typedef enum MAliceType {
        MAliceTypeUndefined = 0,
        MAliceTypeNumber,
        MAliceTypeLetter,
        MAliceTypeSentence
    } MAliceType;

};


#endif // #ifndef _MALICETYPES
