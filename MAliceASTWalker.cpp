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
  pANTLR3_STRING str = printTree(root, 0);
  printf("Analysing File: \n"
         "-----------------------------\n"
         "------------Tree:------------\n"
         "-----------------------------\n"
         "%s", str->chars);
  printf("-----------------------------\n");
  printf("-----Constructing Symbol-----\n");
  printf("------------Table------------\n");
  printf("-----------------------------\n");
  this->visitNode(root, this->rootSymbolTable);
  return true;
}


void MAliceASTWalker :: visitNode(pANTLR3_BASE_TREE node, MAliceSymbolTableNode* symbolTable)
{
  static int calledTimes = 0;
  calledTimes++;
  ANTLR3_UINT32 type = node->getType(node);
  switch (type){
    case (PROGRAM):
      // We must be the first visit node called, else give an error
      if (calledTimes != 1)
        perror("Error, program not root node\n");
      break;
    default:
      std::string errorMessage = std::string("Unknown Node in AST");
      
  }
}


pANTLR3_STRING MAliceASTWalker :: printTree(pANTLR3_BASE_TREE node, int depth)
{
  int i,i2;
  pANTLR3_STRING string	= node->strFactory->newRaw(node->strFactory);
  int numChildren = 0;
  if (node->children != NULL)
    numChildren = node->children->size(node->children);
//  for (i = 0; i < depth; i++)
//    string->append8	(string, " ");
  //string->append8	(string, "-");
  string->appendS	(string, node->toString(node));
  string->append8	(string, "\n");
  for (i = 0; i < numChildren; i++)
  {
    for (i2 = 0; i2 <= depth; i2++)
    {
      string->append8	(string, "|");
      string->append8	(string, " ");
    }
//    string->append8	(string, "|");
    (string->chars[string->len-1]) = '-';
    //string->append8	(string, "-");
    pANTLR3_BASE_TREE child = (pANTLR3_BASE_TREE)node->children->get(node->children,i);
    string->appendS(string, printTree(child , depth+1));
  }
  return string;
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