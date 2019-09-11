#include "error.hpp"

bool IsEmpty(String errorStr)
{
    errorStr.trim();
    return errorStr == "{}";
}

bool IsError(String errorStr)
{
    errorStr.trim();
    return errorStr.startsWith("{\"error\":");
}

const char * GetErrorMessageFromReturnString(const char * error)
{
    unsigned int errorCode = -1;
    DebugPrintf("Error input: %s\n", error);
    sscanf(error, "{\"error\": %u}", &errorCode);
    DebugPrintf("Deduced error code: %u\n", errorCode);
    return GetErrorMessage(errorCode);
}

const char * GetErrorMessage(unsigned int errorCode)
{
    static char buf[64];
    if(errorCode < ERROR_COUNT)
    {
        return ERROR_MESSAGE_TEXT[errorCode];
    }
    else
    {
       return ERROR_MESSAGE_TEXT[ERROR_COUNT-1];
    }
}