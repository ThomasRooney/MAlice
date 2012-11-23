#ifndef _MALICESYMBOLTABLE
#define _MALICESYMBOLTABLE

#include <unordered_map>
#include <string>
#include "MAliceEntity.h"

class MAliceSymbolTableNode
{
  private:
    std::unordered_map<std::string, MAliceEntity> m_scopeMap;
    MAliceSymbolTableNode *m_parentNode;
  public:
    MAliceSymbolTableNode(MAliceSymbolTableNode *parentNode);
    MAliceEntity* get(std::string);
    MAliceEntity* insert(std::string, MAliceEntity);
    unsigned int size();
    MAliceSymbolTableNode* createChildScope();
    MAliceSymbolTableNode* getChildScope();
    MAliceSymbolTableNode* getParentNode();
};

#endif