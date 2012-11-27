#ifndef _MALICETYPES
#define _MALICETYPES

#include "MAliceParser.h"
#include <ostream>

#define SSTR( x )    ((std::ostringstream&)(std::ostringstream() << x)).str()

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;

    typedef pANTLR3_BASE_TREE ASTNode;
    typedef bool (*MAliceVisitFunction)(ASTNode, ASTWalker*, CompilerContext*);
    
    typedef enum {
        MAliceTypeNone = 0,
        MAliceTypeNumber,
        MAliceTypeLetter,
        MAliceTypeSentence,
        MAliceTypeBoolean
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
