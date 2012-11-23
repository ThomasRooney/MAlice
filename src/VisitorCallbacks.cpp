#include "VisitorCallbacks.h"

namespace MAlice {

void visitProgramNode(ASTNode node, CompilerContext *ctx)
{
  std::cout << "Visited Program Node";
}


};