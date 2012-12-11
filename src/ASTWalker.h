#ifndef _MALICEASTWALKER
#define _MALICEASTWALKER

#include "MAliceLexer.h"
#include "MAliceParser.h"

#include <vector>
#include "CompilerContext.h"
#include "Types.h"

namespace MAlice {

    class ASTWalker {
    public:
        ASTWalker();
        void feedTree();
        bool validateTree(ASTNode node, CompilerContext *ctx);
        bool generateIRFromTree(ASTNode root, CompilerContext *ctx);
        void compileTree();
        ~ASTWalker();
        bool visitNode(ASTNode node, llvm::Value **outValue, CompilerContext *ctx);
        bool validateNode(ASTNode node, CompilerContext *ctx);
        bool visitChildren(ASTNode node, std::vector<llvm::Value*> *childValueLists, CompilerContext *ctx);
        bool validateChildren(ASTNode node, CompilerContext *ctx);
    protected:

    private:
        static std::unordered_map<unsigned int, MAliceVisitFunction> visitDictionary;
        static std::unordered_map<unsigned int, MAliceValidationFunction> validationDictionary;
        void constructVisitDictionary ();

        MAliceVisitFunction getNodeVisitFunction(ASTNode node);
        MAliceValidationFunction getNodeValidationFunction(ASTNode node);
        
    }; // class ASTWalker

}; // namespace MAlice

#endif