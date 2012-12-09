
#include "IdentifierDispenser.h"

#include <sstream>

namespace MAlice {
    
    std::string IdentifierDispenser::identifierForFunctionWithName(std::string functionName)
    {
        unsigned int index = 0;
        
        std::unordered_map<std::string,unsigned int>::iterator element = m_functionNameMap.find(functionName);
        if (element == m_functionNameMap.end())
            m_functionNameMap.insert(std::make_pair(functionName, 0));
        else {
            index = element->second;
            m_functionNameMap.insert(std::make_pair(functionName, index + 1));
        }
        
        if (index == 0)
            return functionName;
        
        std::stringstream stringStream(functionName);
        stringStream << index;
        
        return stringStream.str();
    }
    
} // namespace MAlice