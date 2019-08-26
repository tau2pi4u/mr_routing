#include "error.hpp"

bool IsError(const String & errorStr)
{
    errorStr.trim();
    return errorStr.StartsWith("{\"error\":");
}

const char * GetErrorMessageFromReturnString(const char * error)
{
    unsigned int errorCode = -1;
    sscanf(error, "{\"error\": \"%u\"}", &errorCode);
    return GetErrorMessage(errorCode);
}

const char * GetErrorMessage(unsigned int errorCode)
{
    static char buf[64];
    if(errorCode < ERROR_COUNT)
    {
        strcpy_P(buf, (char*)pgm_read_word(&ERROR_MESSAGE_TEXT[errorCode]));
    }
    else
    {
       strcpy_P(buf, (char*)pgm_read_word(&ERROR_MESSAGE_TEXT[ERROR_COUNT-1]));
    }
    return buf;
}