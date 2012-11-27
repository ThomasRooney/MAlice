
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
    }
    
    ErrorType Error::getType()
    {
        return m_errorType;
    }
    
    unsigned int Error::getLineNumber()
    {
        return m_lineNumber;
    }
    
    void Error::setLineNumber(unsigned int lineNumber)
    {
        m_lineNumber = lineNumber;
    }
    
    std::string Error::getErrorMessage()
    {
        return m_errorMessage;
    }
    
    void Error::setErrorMessage(std::string errorMessage)
    {
        m_errorMessage = errorMessage;
    }
    
    std::list<Range*> Error::getUnderlineRanges()
    {
        return m_underlineRanges;
    }
    
    void Error::setUnderlineRanges(std::list<Range*> underlineRanges)
    {
        m_underlineRanges = underlineRanges;
    }
    
    std::list<Range*> Error::getArrowRanges()
    {
        return m_arrowRanges;
    }
    
    void Error::setArrowRanges(std::list<Range*> arrowRanges)
    {
        m_arrowRanges = arrowRanges;
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