#ifndef _MALICEASTWALKER
#define _MALICEASTWALKER

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include <vector>
#include "CompilerContext.h"

#define NUMBER_OF_VISIT_FUNCTIONS 34

namespace MAlice {

typedef pANTLR3_BASE_TREE ASTNode;

class ASTWalker {
public:
  ASTWalker();
  void feedTree();
  bool validateTree(ASTNode node, CompilerContext *ctx);
  void compileTree();
  pANTLR3_STRING printTree(ASTNode node, int);
  ~ASTWalker();
protected:

private:
  void visitNode(ASTNode node, CompilerContext *ctx);
  std::unordered_map<unsigned int,void (*)(ASTNode node, CompilerContext*)> visitDictionary;
  void constructVisitDictionary ();
}; // class ASTWalker




}; // namespace MAlice

#endif