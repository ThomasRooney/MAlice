#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <string>
#include <sstream>

#include "ASTWalker.h"

#include "MAliceParser.h"
#include "SymbolTable.h"
#include "Utilities.h"
#include "VisitorCallbacks.h"

namespace MAlice {

ASTWalker :: ASTWalker () {
  constructVisitDictionary();
}
    
ASTWalker :: ~ASTWalker() {
}

void ASTWalker :: constructVisitDictionary() {
  static bool doOnce = false;
  if (!doOnce) {
    doOnce = true;
    visitDictionary = std::unordered_map<unsigned int,void(*)(ASTNode, CompilerContext*)>();
    visitDictionary.insert(
      std::make_pair
      (PROGRAM, &visitProgramNode)
      );
    
/*             ALICEFOUND, 
             ALICEFOUND      4
             ALICEWASUNSURE      5
             AND      6
             APOSTROPHE_S      7
             ARRAY      8
             ARRAYSUBSCRIPT      9
             ASSIGNMENTSTATEMENT      10
             ATE      11
             BANG      12
             BECAME      13
             BECAUSE      14
             BITWISEAND      15
             BITWISEOR      16
             BITWISEXOR      17
             BODY      18
             BUT      19
             BYREFERENCE      20
             BYVALUE      21
             CHARACTER_LITERAL      22
             CLOSED      23
             COMMA      24
             COMMENT      25
             CONTAINEDA      26
             DECLS      27
             DECREMENTSTATEMENT      28
             DIGIT      29
             DIVIDE      30
             DRANK      31
             EITHER      32
             ENOUGHTIMES      33
             EQUALS      34
             ESCAPE      35
             EVENTUALLY      36
             EXPRESSION      37
             FULL_STOP      38
             FUNCDEFINITION      39
             GREATERTHAN      40
             GREATERTHANEQUAL      41
             HAD      42
             IDENTIFIER      43
             IFSTATEMENT      44
             INCREMENTSTATEMENT      45
             INPUTSTATEMENT      46
             INVOCATION      47
             LESSTHAN      48
             LESSTHANEQUAL      49
             LETTER      50
             LETTER_TYPE      51
             LOGICALAND      52
             LOGICALOR      53
             LPAREN      54
             MAYBE      55
             MINUS      56
             MODULO      57
             MULTIPLY      58
             NEWLINE      59
             NOTEQUAL      60
             NUMBER_LITERAL      61
             NUMBER_TYPE      62
             OF      63
             OPENED      64
             OR      65
             PARAMS      66
             PERHAPS      67
             PIECE      68
             PLUS      69
             PRINTSTATEMENT      70
             PROCDEFINITION      71
             PROGRAM      72
             QUESTION_MARK      73
             QUOTE      74
             RETURNSTATEMENT      75
             RPAREN      76
             SAIDALICE      77
             SENTENCE_TYPE      78
             SO      79
             SPIDER      80
             SPOKE      81
             STATEMENTLIST      82
             STRING_LITERAL      83
             THELOOKINGGLASS      84
             THEN      85
             THEROOM      86
             TILDE      87
             TOO      88
             UNDERSCORE      89
             WASA      90
             WHATWAS      91
             WHILESTATEMENT      92
             WS      93*/
  }
}

bool ASTWalker::validateTree(pANTLR3_BASE_TREE root, CompilerContext *ctx) {
  // Get the root node, it should be program, if not then error
  printf("-----------------------------\n");
  printf("-----Constructing Symbol-----\n");
  printf("------------Table------------\n");
  printf("-----------------------------\n");

  this->visitNode(root, ctx);

  return true;
}


void ASTWalker::visitNode(ASTNode node, CompilerContext *ctx)
{
    MAliceVisitFunction f = getNodeVisitFunction(node);
    
    // If we haven't implemented visitor functions for certain node types (e.g. nodes which we don't do anything
    // with but are there to make the AST nicer to work with), simply recurse on their children and vist them.
    if (f == NULL)
        visitChildren(node, ctx);
    
    f(node, ctx);
		
}
    
void ASTWalker::visitChildren(ASTNode node, CompilerContext *ctx)
{
    unsigned int numChildren = Utilities::getNumberOfChildNodes(node);
    
    for (unsigned int i = 0; i < numChildren; ++i) {
        visitNode(node, ctx);
    }
}
    
MAliceVisitFunction ASTWalker::getNodeVisitFunction(ASTNode node)
{
    MAliceVisitFunction f = NULL;
    
    try {
        f = (visitDictionary.at(Utilities::getNodeType(node)));
    }
    catch (std::out_of_range e) {
        std::stringstream error;
        error << "FATAL ERROR - Node: " << (node->toString(node)->chars) << " Type: " << node->getType(node);
        error << " At location(" << node->getToken(node)->getLine(node->getToken(node)) << "," << node->getToken(node)->getCharPositionInLine(node->getToken(node)) << ") has undefined visit function" << "\n\nExitting...";
        std::cerr << error.str();
        exit(0);
    }
    
    return f;
}

void ASTWalker :: compileTree()
{

}

}; // namespace MAlice