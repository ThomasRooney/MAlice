#ifndef _MALICETYPES
#define _MALICETYPES

#include "MAliceParser.h"
#include <ostream>

#include "llvm/Value.h"

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;

    typedef pANTLR3_BASE_TREE ASTNode;
    typedef bool (*MAliceVisitFunction)(ASTNode, llvm::Value**, ASTWalker*, CompilerContext*);
    
    typedef enum {
        MAliceTypeNone = 1 << 0,
        MAliceTypeNumber = 1 << 1,
        MAliceTypeLetter = 1 << 2,
        MAliceTypeSentence = 1 << 3,
        MAliceTypeBoolean = 1 << 4
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
