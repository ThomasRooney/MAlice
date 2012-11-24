#ifndef _MALICEASTWALKER
#define _MALICEASTWALKER

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include <vector>
#include "CompilerContext.h"
#include "Types.h"

#define NUMBER_OF_VISIT_FUNCTIONS 34

namespace MAlice {

class ASTWalker {
public:
  ASTWalker();
  void feedTree();
  bool validateTree(ASTNode node, CompilerContext *ctx);
  void compileTree();
  ~ASTWalker();
protected:

private:
  void visitNode(ASTNode node, CompilerContext *ctx);
  std::unordered_map<unsigned int,void (*)(ASTNode node, CompilerContext*)> visitDictionary;
  void constructVisitDictionary ();
}; // class ASTWalker




}; // namespace MAlice

#endif