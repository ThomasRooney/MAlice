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
        bool generateCodeForNode(ASTNode node, llvm::Value **outValue, CompilerContext *ctx);
        bool validateNode(ASTNode node, CompilerContext *ctx);
        bool generateCodeForChildren(ASTNode node, std::vector<llvm::Value*> *childValueLists, CompilerContext *ctx);
        bool validateChildren(ASTNode node, CompilerContext *ctx);
    protected:

    private:
        static std::unordered_map<unsigned int, MAliceCodeGenerationFunction> codeGenerationDictionary;
        static std::unordered_map<unsigned int, MAliceValidationFunction> validationDictionary;
        void constructCodeGenerationDictionary();

        MAliceCodeGenerationFunction getNodeCodeGenerationFunction(ASTNode node);
        MAliceValidationFunction getNodeValidationFunction(ASTNode node);
        
    }; // class ASTWalker

}; // namespace MAlice

#endif