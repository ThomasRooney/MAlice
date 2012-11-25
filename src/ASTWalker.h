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
        bool visitNode(ASTNode node, CompilerContext *ctx);
        bool visitChildren(ASTNode node, CompilerContext *ctx);
    protected:

    private:
        std::unordered_map<unsigned int, MAliceVisitFunction> visitDictionary;
        void constructVisitDictionary ();
        MAliceVisitFunction getNodeVisitFunction(ASTNode node);
    }; // class ASTWalker

}; // namespace MAlice

#endif