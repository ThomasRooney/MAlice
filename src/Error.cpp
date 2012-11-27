
#include "Error.h"

namespace MAlice {
    
    Error::Error(ErrorType type)
    {
        Error(type, "");
    }
    
    Error::Error(ErrorType type, std::string errorMessage)
    {
        m_errorType = type;
        
        if (!errorMessage.empty())
            m_errorMessage = errorMessage;
        
        m_errorPosition = NULL;
        m_range = NULL;
    }
    
    Error::~Error()
    {
        if (m_range) {
            delete m_range;
            m_range = NULL;
        }
        
        if (m_errorPosition) {
            delete m_errorPosition;
            m_errorPosition = NULL;
        }
    }
    
    ErrorType Error::getType()
    {
        return m_errorType;
    }
    
    ErrorPosition *Error::getErrorPosition()
    {
        return m_errorPosition;
    }
    
    void Error::setErrorPosition(ErrorPosition *errorPosition)
    {
        m_errorPosition = errorPosition;
    }
    
    Range *Error::getRange()
    {
        return m_range;
    }
    
    void Error::setRange(Range *range)
    {
        m_range = range;
    }
    
    std::string Error::getErrorMessage()
    {
        return m_errorMessage;
    }
    
    void Error::setErrorMessage(std::string errorMessage)
    {
        m_errorMessage = errorMessage;
    }
    
    std::string Error::getAdditionalInformation()
    {
        return m_additionalInformation;
    }
    
    void Error::setAdditionalInformation(std::string additionalInformation)
    {
        m_additionalInformation = additionalInformation;
    }
    
} // namespace MAlice