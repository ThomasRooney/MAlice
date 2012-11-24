#include "VisitorCallbacks.h"

namespace MAlice {

void visitProgramNode(ASTNode node, ASTWalker *walker, CompilerContext *ctx)
{
    walker->visitChildren(node, ctx);
}


};