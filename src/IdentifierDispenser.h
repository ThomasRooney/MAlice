
#ifndef _MALICEIDENTIFIERDISPENSER
#define _MALICEIDENTIFIERDISPENSER

#include <unordered_map>
#include <string>

namespace MAlice {
    
    class IdentifierDispenser {
    private:
        std::unordered_map<std::string, unsigned int> m_functionNameMap;
    public:
        std::string identifierForFunctionWithName(std::string functionName);
    };
    
} // namespace MAlice

#endif