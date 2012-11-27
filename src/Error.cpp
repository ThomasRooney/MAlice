
#include "Error.h"

namespace MAlice {
    
    Error::Error(ErrorType type)
    {
        m_errorType = type;
    }
    
    Error::Error(ErrorType type, std::string errorMessage)
    {
        m_errorType = type;
        m_errorMessage = errorMessage;
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