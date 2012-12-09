
#ifndef _MALICEUTILITIES
#define _MALICEUTILITIES

#include <iostream>
#include <sstream>
#include <unordered_map>

#include "Types.h"

namespace MAlice {
    
    class Entity;
    class ErrorPosition;
    class SymbolTable;
    class ParameterEntity;
    class FunctionProcedureEntity;
    
    class Utilities {
    private:
        static pANTLR3_STRING _printTree(ASTNode node, int depth);
        
        static void createTokenIdentifierToTextMap();
    public:
        static unsigned int getNodeLineNumber(ASTNode node);
        static unsigned int getNodeColumnIndex(ASTNode node);
        static std::list<Range*> rangeToSingletonList(Range *r);
        static void printTree(ASTNode tree);
        
        static ANTLR3_UINT32 getNodeType(ASTNode node);
        static unsigned int getNumberOfChildNodes(ASTNode node);
        static ASTNode getChildNodeAtIndex(ASTNode node, unsigned int index);
        static char *getNodeText(ASTNode node);
        static MAliceType getTypeFromTypeString(std::string);
        static MAliceType getTypeFromNodeType(int nodeType);
        static const char* getNameOfTypeFromMAliceType(MAliceType);
        static MAliceEntityType Utilities::getTypeOfEntity(Entity *entity);
        static bool isKindOfEntity(Entity *entity, MAliceEntityType type);
        static std::string getTokenTextFromTokenIdentifier(ANTLR3_UINT32 tokenIdentifier);
        
        static std::string getNodeTextIncludingChildren(ASTNode node, CompilerContext *ctx, Range **outRange);
        static ASTNode getLeftDeepestChildNode(ASTNode node);
        static ASTNode getRightDeepestChildNode(ASTNode node);
        
        static bool isImaginaryNode(ASTNode node);
        static ASTNode getFirstNonImaginaryChildNode(ASTNode node);
        
        static std::string stringWithLineIndentation(std::string string, unsigned int lineIndentation);
        
        static ErrorPosition *getErrorPositionFromNode(ASTNode node);
        static Range *createRange(unsigned int startLine, unsigned int startColumn);
        static Range *createRange(unsigned int startLine, unsigned int startColumn, unsigned int endColumn);
        static Range *createRange(unsigned int startLine, unsigned int startColumn, unsigned int endLine, unsigned int endColumn);
        
        static std::string stripLeadingAndTrailingCharacters(std::string input, char character);
        static std::string getOperatorStringFromOperatorNode(ASTNode node);
        static std::string getTypeListFromTypeFlags(unsigned int flags);
        
        template <typename T>
        static std::string numberToString(T number) {
            std::ostringstream stream;
            stream << number;
            
            return stream.str();
        }
        
        static void printSymbolTableEntries(SymbolTable *symbolTable);
        static std::vector<ParameterEntity*> getParameterTypesFromParamsNode(ASTNode paramsNode);
        static FunctionProcedureEntity *getFunctionProcedureEntityForInvocationNode(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        static MAliceType getReturnTypeForInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        static std::string getFunctionProcedureInvocationIdentifier(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromInvocationExpressionNode(ASTNode node, MAliceType *outType, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromExpressionNode(ASTNode node, MAliceType *outType, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, bool *passedByReference);
        static bool getTypeFromExpressionRuleNode(ASTNode node, MAliceType *outType, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromBinaryOperatorNode(ASTNode node, MAliceType *outType, std::string operatorName, unsigned int requiredTypes, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromUnaryOperatorNode(ASTNode node, MAliceType *outType, std::string operatorName, MAliceType requiredType, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromExpressionIdentifierNode(ASTNode node, MAliceType *outType, MAliceEntityType *outEntityType, ASTWalker *walker, CompilerContext *ctx);
        
        static llvm::Type *getLLVMTypeFromMAliceType(MAliceType type);
    };
    
}; // namespace MAlice

#endif /* #ifndef _MALICEUTILITIES */
