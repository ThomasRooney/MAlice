
#ifndef _MALICEERROR
#define _MALICEERROR

#include <iostream>
#include <list>

#include "Types.h"

namespace MAlice {
    
    class Range;
    class ErrorPosition;
    
    class Error {
    private:
        ErrorType m_errorType;
        unsigned int m_lineNumber;
        std::list<Range*> m_underlineRanges;
        std::list<Range*> m_arrowRanges;
        
        std::string m_errorMessage;
        std::string m_additionalInformation;
        
    public:
        Error(ErrorType type);
        Error(ErrorType type, std::string errorMessage);
        
        ErrorType getType();
        unsigned int getLineNumber();
        std::string getErrorMessage();
        std::list<Range*> getUnderlineRanges();
        std::list<Range*> getArrowRanges();
        std::string getAdditionalInformation();
        
        void setLineNumber(unsigned int lineNumber);
        void setUnderlineRanges(std::list<Range*> ranges);
        void setArrowRanges(std::list<Range*> ranges);
        void setErrorMessage(std::string errorMessage);
        void setAdditionalInformation(std::string additionalInformation);
    };
    
} // namespace MAlice

#endif /* #ifndef _MALICEERROR */
