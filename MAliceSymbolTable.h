#include <unordered_map>


class MALiceSymbolTableNode
{
  private:
    std::unordered_map<std::string, MALiceEntity> scopeMap;
  public:
    MALiceEntity* get(std::string);
    MALiceEntity* insert(std::string, MALiceEntity);
    unsigned int size();
    MALiceSymbolTableNode* createChildScope();
    MALiceSymbolTableNode* getChildScope();
    MALiceSymbolTableNode* getParentNode();
}
