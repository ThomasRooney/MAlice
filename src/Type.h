
#ifndef _MALICETYPE
#define _MALICETYPE

#include <iostream>

namespace MAlice {
    
    typedef enum {
        PrimitiveTypeNone = 1 << 0,
        PrimitiveTypeNumber = 1 << 1,
        PrimitiveTypeLetter = 1 << 2,
        PrimitiveTypeSentence = 1 << 3,
        PrimitiveTypeBoolean = 1 << 4
    } PrimitiveType;
    
    class Type {
    private:
        PrimitiveType m_primitiveType;
        bool m_isArray;
    public:
        Type();
        Type(PrimitiveType t);
        
        static Type arrayType(PrimitiveType t);
        static Type voidType();
        
        PrimitiveType getPrimitiveType();
        bool isArray();
        void setIsArray(bool isArray);
        
        bool isVoid();
        
        bool operator==(Type t2);
        bool operator!=(Type t2);
    };
    
} // namespace MAlice

#endif // #ifndef _MALICETYPE
