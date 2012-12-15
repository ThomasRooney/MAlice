
#include "StringTable.h"

namespace MAlice {

    llvm::Value *StringTable::cachedStringConstant(std::string constant)
    {
        if (m_stringTable.find(constant) != m_stringTable.end())
            return m_stringTable.at(constant);
        
        return NULL;
    }
    
    void StringTable::cacheStringConstant(std::string constant, llvm::Value *value)
    {
        m_stringTable.insert(std::pair<std::string, llvm::Value*>(constant, value));
    }
    
    void StringTable::removeAllEntries()
    {
        m_stringTable.clear();
    }

} // namespace MAlice