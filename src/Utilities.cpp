
#include "Utilities.h"

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
            string->appendS(string, _printTree(child , depth+1));
        }
        
        return string;
    }
    
}; // namespace MAlice