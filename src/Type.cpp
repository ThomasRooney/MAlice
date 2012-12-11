
#include "Type.h"

namespace MAlice {
    
    Type::Type()
    {
        m_primitiveType = PrimitiveTypeNone;
        m_isArray = false;
    }
    
    Type::Type(PrimitiveType t)
    {
        m_primitiveType = t;
        m_isArray = false;
    }
    
    Type Type::arrayType(PrimitiveType t)
    {
        Type type(t);
        type.setIsArray(true);
        
        return type;
    }
    
    Type Type::voidType()
    {
        return Type(PrimitiveTypeNone);
    }
    
    PrimitiveType Type::getPrimitiveType()
    {
        return m_primitiveType;
    }
    
    bool Type::isArray()
    {
        return m_isArray;
    }
                 
    void Type::setIsArray(bool isArray)
    {
        m_isArray = isArray;
    }
    
    bool Type::operator==(Type t2)
    {
        return getPrimitiveType() == t2.getPrimitiveType() && isArray() == t2.isArray();
    }
    
    bool Type::operator!=(Type t2)
    {
        return !(getPrimitiveType() == t2.getPrimitiveType() && isArray() == t2.isArray());
    }
    
    bool Type::isVoid()
    {
        return getPrimitiveType() == PrimitiveTypeNone;
    }
    
}
