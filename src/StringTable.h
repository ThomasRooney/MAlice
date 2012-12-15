
#ifndef _MALICESTRINGTABLE
#define _MALICESTRINGTABLE

#include <iostream>
#include <unordered_map>

#include "llvm/Value.h"
#include "llvm/Module.h"

namespace MAlice {
    
    class StringTable {
    private:
        std::unordered_map<std::string, llvm::Value*> m_stringTable;
        
    public:
        llvm::Value *cachedStringConstant(std::string constant);
        void cacheStringConstant(std::string constant, llvm::Value *value);
        
        void removeAllEntries();
    }; // class StringTable
    
} // namespace MAlice

#endif // #ifndef _MALICESTRINGTABLE
