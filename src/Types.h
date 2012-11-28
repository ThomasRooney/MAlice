#ifndef _MALICETYPES
#define _MALICETYPES

#include "MAliceParser.h"
#include <ostream>

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;

    typedef pANTLR3_BASE_TREE ASTNode;
    typedef bool (*MAliceVisitFunction)(ASTNode, ASTWalker*, CompilerContext*);
    
    typedef enum {
        MAliceTypeNone = 0,
        MAliceTypeNumber = 1 << 0,
        MAliceTypeLetter = 1 << 1,
        MAliceTypeSentence = 1 << 2,
        MAliceTypeBoolean = 1 << 3
    } MAliceType;
    
    typedef enum {
        MAliceEntityTypeUndefined = 0,
        MAliceEntityTypeVariable,
        MAliceEntityTypeFunction,
        MAliceEntityTypeProcedure,
        MAliceEntityTypeParameter,
        MAliceEntityTypeArray,
    } MAliceEntityType;

};


#endif // #ifndef _MALICETYPES
