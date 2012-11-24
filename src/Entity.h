
#ifndef _MALICEENTITY
#define _MALICEENTITY

#include <string>

namespace MAlice {
    
    class Entity {
    private:
        std::string m_identifier;
        unsigned int m_lineNumber;
        
    public:
        Entity(std::string identifier, unsigned int lineNumber);
        
        std::string getIdentifier();
        unsigned int getLineNumber();
    }; // class Entity
    
}; // namespace MAlice

#endif