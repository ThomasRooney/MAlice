#ifndef _MALICETYPES
#define _MALICETYPES

#include "MAliceParser.h"
#include "llvmHeader.h"
#include <ostream>
#include <vector>

class Value;

namespace MAlice {
    
    class ASTWalker;
    class CompilerContext;

    typedef pANTLR3_BASE_TREE ASTNode;
    typedef std::vector<llvm::Value*> ValueList;
    typedef bool (*MAliceVisitFunction)(ASTNode, llvm::Value **outValue, ASTWalker*, CompilerContext*);
    typedef bool (*MAliceValidationFunction)(ASTNode, ASTWalker*, CompilerContext*);
    
    typedef enum {
        MAliceTypeNone = 1 << 0,
        MAliceTypeNumber = 1 << 1,
        MAliceTypeLetter = 1 << 2,
        MAliceTypeSentence = 1 << 3,
        MAliceTypeBoolean = 1 << 4
    } MAliceType;
    
    typedef enum {
        MAliceEntityTypeUndefined = 1 << 0,
        MAliceEntityTypeVariable = 1 << 1,
        MAliceEntityTypeFunction = 1 << 2,
        MAliceEntityTypeProcedure = 1 << 3,
        MAliceEntityTypeParameter = 1 << 4,
        MAliceEntityTypeGlobalVariable = 1 << 5,
        MAliceEntityTypeArray = 1 << 6,
    } MAliceEntityType;

};


#endif // #ifndef _MALICETYPES
