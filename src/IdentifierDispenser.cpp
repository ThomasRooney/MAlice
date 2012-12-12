
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
        
        // Prefix with underscores to denote a user-defined function. Helps to prevent collisions with our own
        // functions and the @main entry point function.
        std::stringstream stringStream("_" + functionName);
        stringStream << index;
        
        return stringStream.str();
    }
    
    std::string IdentifierDispenser::identifierForEntryPointFunction()
    {
        return "main";
    }
    
} // namespace MAlice