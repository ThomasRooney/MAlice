
#ifndef _MALICEUTILITIES
#define _MALICEUTILITIES

#include <iostream>
#include <sstream>
#include <unordered_map>

#include "Types.h"
#include "Type.h"

#ifdef _WIN32
#define DELIM '\\'
#else
#define DELIM '/'
#endif

namespace MAlice {
    
    class Entity;
    class ErrorPosition;
    class SymbolTable;
    class ParameterEntity;
    class FunctionProcedureEntity;
    class Type;
    
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
        static Type getTypeFromTypeString(std::string);
        static Type getTypeFromNodeType(int nodeType);
        static std::string getNameOfPrimitiveType(Type type);
        static MAliceEntityType getTypeOfEntity(Entity *entity);
        static bool isKindOfEntity(Entity *entity, MAliceEntityType type);
        static std::string getTokenTextFromTokenIdentifier(ANTLR3_UINT32 tokenIdentifier);
        
        static std::string getNodeTextIncludingChildren(ASTNode node, CompilerContext *ctx, Range **outRange);
        static ASTNode getLeftDeepestChildNode(ASTNode node);
        static ASTNode getRightDeepestChildNode(ASTNode node);
        
        static bool isImaginaryNode(ASTNode node);
        static ASTNode getFirstNonImaginaryChildNode(ASTNode node);
        
        static std::string stringWithLineIndentation(std::string string, unsigned int lineIndentation);
        
        static ErrorPosition *getErrorPositionFromNode(ASTNode node);
        
        static std::string stripLeadingAndTrailingCharacters(std::string input, char character);
        static std::string getOperatorStringFromOperatorNode(ASTNode node);
        static std::string getTypeListFromTypes(std::vector<Type> types);
        
        static std::string getParentDirectoryForPath(std::string path);
        static std::string getBaseFilenameFromPath(std::string path);
        
        template <typename T>
        static std::string numberToString(T number) {
            std::ostringstream stream;
            stream << number;
            
            return stream.str();
        }
        
        template <typename T>
        static std::vector<T> singletonVector(T type) {
            std::vector<T> vec;
            vec.push_back(type);
            
            return vec;
        }
        
        static std::vector<ParameterEntity*> getParameterTypesFromParamsNode(ASTNode paramsNode);
        static FunctionProcedureEntity *getFunctionProcedureEntityForInvocationNode(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        static Type getReturnTypeForInvocation(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        static std::string getFunctionProcedureInvocationIdentifier(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromInvocationExpressionNode(ASTNode node, Type *outType, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromExpressionNode(ASTNode node, Type *outType, bool requiresLValue, ASTWalker *walker, CompilerContext *ctx, bool *passedByReference);
        static bool getTypeFromExpressionRuleNode(ASTNode node, Type *outType, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromBinaryOperatorNode(ASTNode node, Type *outType, std::string operatorName, std::vector<Type> requiredTypes, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromUnaryOperatorNode(ASTNode node, Type *outType, std::string operatorName, Type requiredType, ASTWalker *walker, CompilerContext *ctx);
        static bool getTypeFromExpressionIdentifierNode(ASTNode node, Type *outType, MAliceEntityType *outEntityType, ASTWalker *walker, CompilerContext *ctx, bool *passedByReference);
        
        static llvm::Type *getLLVMTypeFromType(Type type);
        static llvm::Function *getPrintfFunction(llvm::Module *module);
    };
    
}; // namespace MAlice

#endif /* #ifndef _MALICEUTILITIES */
