
#include "Utilities.h"
#include "CompilerContext.h"
#include "ProcedureEntity.h"
#include "FunctionEntity.h"
#include <stdexcept>
#include <typeinfo>
#include "VisitorCallbacks.h"

namespace MAlice {
    
    unsigned int Utilities::getNodeLineNumber(ASTNode node)
    {
        pANTLR3_COMMON_TOKEN token = node->getToken(node);
        return token->getLine(token);
    }
    
    unsigned int Utilities::getNodeColumnIndex(ASTNode node)
    {
        pANTLR3_COMMON_TOKEN token = node->getToken(node);
        return token->getCharPositionInLine(token);
    }
    
    void Utilities::printTree(ASTNode tree)
    {
        std::cout << _printTree(tree, 0)->chars;
    }
    
    pANTLR3_STRING Utilities::_printTree(ASTNode node, int depth)
    {
        pANTLR3_STRING string = node->strFactory->newRaw(node->strFactory);
        
        unsigned int numChildren = getNumberOfChildNodes(node);
        
        string->appendS	(string, node->toString(node));
        string->append8	(string, "\n");
        
        for (unsigned int childIndex = 0; childIndex < numChildren; ++childIndex)
        {
            for (int depthIndex = 0; depthIndex <= depth; ++depthIndex)
            {
                string->append8(string, "|");
                string->append8(string, " ");
            }

            string->chars[string->len-1] = '-';

            ASTNode child = getChildNodeAtIndex(node, childIndex);
            string->appendS(string, _printTree(child , depth+1));
        }
        
        return string;
    }
    
    ANTLR3_UINT32 Utilities::getNodeType(ASTNode node)
    {
        return node->getType(node);
    }
    
    unsigned int Utilities::getNumberOfChildNodes(ASTNode node)
    {
        pANTLR3_VECTOR children = node->children;
        if (children == NULL)
            return 0;
        
        return children->size(children);
    }
    
    ASTNode Utilities::getChildNodeAtIndex(ASTNode node, unsigned int index)
    {
        if (index >= Utilities::getNumberOfChildNodes(node))
            return NULL;
        
        pANTLR3_VECTOR children = node->children;
        return (ASTNode)children->get(children, index);
    }
    
    char * Utilities::getNodeText(ASTNode node)
    {
        return (char*)node->getText(node)->chars;
    }
    
    MAliceType Utilities::getTypeFromTypeString(std::string typeString)
    {
        if (!typeString.compare("number"))
            return MAliceTypeNumber;
        
        if (!typeString.compare("letter"))
            return MAliceTypeLetter;
        
        if (!typeString.compare("sentence"))
            return MAliceTypeSentence;
        
        return MAliceTypeUndefined;
    }

    bool Utilities::confirmTypeOfExpression(ASTNode node, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm)
    {
        int numChildren = Utilities::getNumberOfChildNodes(node);
        bool typeCheck = true;
        // Are we at a bottom node yet?
        if (numChildren == 0)
        {
            // If so, check type
            std::string info = Utilities::getNodeText(node);
            int type = Utilities::getNodeType(node);
            MAliceType maType;
            
            if (type == IDENTIFIER)
            {
                Entity * lookupEntity;
                if (ctx->isSymbolInScope(info, &lookupEntity))
                {
                    VariableEntity *lookupVEntity = NULL;
                    try {
                        lookupVEntity = dynamic_cast<VariableEntity *>(lookupEntity);
                        if (lookupVEntity == NULL)
                          throw std::bad_cast();
                        maType = lookupVEntity->getType();
                    }
                    catch (std::bad_cast e){
                        ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                                             Utilities::getNodeColumnIndex(node),
                                                             ErrorType::Semantic,
                                                             "Identifier: '" + info + "' is not a variable.",
                                                             false);
                        return false;
                    }

                }
                else {
                    ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                        Utilities::getNodeColumnIndex(node),
                                        ErrorType::Semantic,
                                        "Identifier: '" + info + "' is not in scope.",
                                        false);
                    return false;
                }
            }
            else {
              maType = Utilities::getTypeFromNodeType(type);
            }
            
            typeCheck = maType == typeConfirm;
            if (!typeCheck)
            {
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(node),
                                          Utilities::getNodeColumnIndex(node),
                                          ErrorType::Semantic,
                                          "Expression: '" + info + "' is not of expected type: (\'" \
                                            + Utilities::getNameOfTypeFromMAliceType(typeConfirm) + "\' != \'" +
                                            Utilities::getNameOfTypeFromMAliceType(maType) + "\')",
                                          false);

            }
            return typeCheck;
        }
        else 
        {
            for (;numChildren > 0; numChildren--)
            {
                // get the node
                ASTNode childNode = Utilities::getChildNodeAtIndex(node,numChildren-1);
                // if childNode is an invocationnode, check symbol exists and arguments are correcgt and typecheck
                if (Utilities::getNodeType(childNode) == INVOCATION)
                {
                    typeCheck = Utilities:: checkIsValidInvocationAndReturnType(childNode, walker, ctx, typeConfirm);
                }
                else {
                    typeCheck = Utilities::confirmTypeOfExpression(childNode,walker,ctx,typeConfirm) && typeCheck;
                }
            }
        }
        return typeCheck;
    }

    MAliceType Utilities::getTypeFromNodeType(int nodeType) 
    {
        switch (nodeType)
        {
            case STRING_LITERAL:
                return MAliceTypeSentence;
            case CHARACTER_LITERAL:
                return MAliceTypeLetter;
            case NUMBER_LITERAL:
                return MAliceTypeNumber;
            default:
                return MAliceTypeUndefined;
        }        
    }

    const char* Utilities::getNameOfTypeFromMAliceType(MAliceType type)
    {
        int t = type;
        //TODO: Move me somewhere better (types.h no good as linker error)
        const char* MAliceTypeNames [] = {
            "Undefined Type",
            "Number Type",
            "Letter Type",
            "Sentence Type",
            };
        if (t < 0 || t >= 4)
          t = 0;
        return (MAliceTypeNames [t]);
    }

    bool Utilities:: checkIsValidInvocationAndReturnType(ASTNode invocationNode, ASTWalker *walker, CompilerContext *ctx, MAliceType typeConfirm)
    {
        ASTNode identNode = Utilities::getChildNodeAtIndex(invocationNode,0);
        // does childNode return the right type, and is it in scope
        std::string ident = Utilities::getNodeText(identNode);
        Entity *entityBuffer = NULL;
        FunctionEntity *functionEntity;
        ProcedureEntity *procedureEntity;
        // Get the entity referred to by the ident, error if its not in scope
        if (ctx->isSymbolInScope(ident, &entityBuffer))
        {
            // Make sure its not a procedure first
            try {
                procedureEntity = dynamic_cast<ProcedureEntity *>(entityBuffer);
                if (procedureEntity != NULL)
                    ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(invocationNode),
                                            Utilities::getNodeColumnIndex(invocationNode),
                                            ErrorType::Semantic,
                                            "No return type for procedure: \'" + ident + "\' called - Use a function",
                                            false);

            }
            catch (std::bad_cast e)
            {
                // Good, its not a procedure
            }
            try {
                functionEntity = dynamic_cast<FunctionEntity *>(entityBuffer);
                if (functionEntity == NULL)
                    throw std::bad_cast();
                // Check the return type
                MAliceType returnType = functionEntity->getReturnType();
                if (returnType != typeConfirm)
                    ctx->getErrorReporter()->reportError(
                        Utilities::getNodeLineNumber(invocationNode),
                        Utilities::getNodeColumnIndex(invocationNode),
                        ErrorType::Semantic,
                        "Function: '" + ident + "' does not return the expected type: (\'" \
                        + Utilities::getNameOfTypeFromMAliceType(typeConfirm) + "\' != \'" +
                        Utilities::getNameOfTypeFromMAliceType(returnType) + "\')",
                        false);
                    
            }
            catch (std::bad_cast e)
            {
                // this should never happen
                ctx->getErrorReporter()->reportError(Utilities::getNodeLineNumber(invocationNode),
                                        Utilities::getNodeColumnIndex(invocationNode),
                                        ErrorType::Semantic,
                                        "Malformed Symbol Table - Invocation Node refers to a non-functional node!",
                                        true);
            }
            
        }

        return true;
    }
    
}; // namespace MAlice