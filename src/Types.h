//
//  Types.h
//  MAlice
//
//  Created by Alex Rozanski on 23/11/2012.
//
//

#ifndef _MALICETYPES
#define MAlice_Types_h

#include "MAliceParser.h"
#include "CompilerContext.h"

namespace MAlice {

    typedef pANTLR3_BASE_TREE ASTNode;
    typedef void (*MAliceVisitFunction)(ASTNode, CompilerContext*);

};


#endif
