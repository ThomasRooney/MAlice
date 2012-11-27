
#ifndef _MALICEERRORHELPERS
#define _MALICEERRORHELPERS

#include <iostream>

#include "MAliceParser.h"
#include "Types.h"

namespace MAlice {

    class CompilerContext;
    
    void outputInvalidASTError(CompilerContext *ctx, std::string currentValidationType);
    void outputInvalidLValueError(CompilerContext *ctx, ASTNode node);
    
};

#endif /* #ifndef _MALICEERRORHELPERS */
