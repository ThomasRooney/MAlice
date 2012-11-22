#include "MAliceASTWalker.h"
#include "MAliceSymbolTable.h"

MAliceASTWalker :: MAliceASTWalker (){
  rootSymbolTable = NULL;
}
MAliceASTWalker :: ~MAliceASTWalker() {
  if (rootSymbolTable != NULL)
  {
    delete rootSymbolTable;
  }
}

bool MAliceASTWalker  :: validateTree(pANTLR3_BASE_TREE root) {
  initialiseSymbolTable();

  // Get the root node, it should be program, if not then error
  printTree(root);
  return true;
}

void MAliceASTWalker :: printTree(pANTLR3_BASE_TREE root)
{

}

void MAliceASTWalker :: compileTree() {

}

void MAliceASTWalker :: initialiseSymbolTable(){
  if (rootSymbolTable != NULL)
  {
    delete rootSymbolTable;
  }
    rootSymbolTable = new MAliceSymbolTableNode(NULL);

}