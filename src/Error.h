
#ifndef _MALICEERROR
#define _MALICEERROR

#include <iostream>

#include "Range.h"
#include "Types.h"
#include "ErrorPosition.h"

namespace MAlice {
    
    class Error {
    private:
        ErrorType m_errorType;
        ErrorPosition *m_errorPosition;
        Range *m_range;
        std::string m_errorMessage;
        std::string m_additionalInformation;
        
    public:
        Error(ErrorType type);
        Error(ErrorType type, std::string errorMessage);
        ~Error();
        
        ErrorType getType();
        ErrorPosition *getErrorPosition();
        void setErrorPosition(ErrorPosition *errorPosition);
        Range *getRange();
        void setRange(Range *range);
        std::string getErrorMessage();
        void setErrorMessage(std::string errorMessage);
        
        std::string getAdditionalInformation();
        void setAdditionalInformation(std::string additionalInformation);
    };
    
} // namespace MAlice

#endif /* #ifndef _MALICEERROR */
