#include "utils.hpp"

void RemoveCharacterFromString(String & str, char toRemove)
{
    int idx = str.indexOf(toRemove);
    while(idx != -1)
    {
        //Serial.printf("Removing %c from string %s\n", toRemove, str.c_str());
        str.remove(idx, 1);
        idx = str.indexOf(toRemove);
    }
    //Serial.printf("Final string %s\n", str.c_str());
}

void DebugPrintf(const char * format, ...)
{
    #ifdef _DEBUG
    va_list args;
    va_start(args, format);
    Serial.printf(format, args);
    #endif
}

void DebugPrintln(const String & str)
{
    #ifdef _DEBUG
    Serial.println(str);
    #endif
}

void DebugPrint(const String & str)
{
    #ifdef _DEBUG
    Serial.print(str);
    #endif
}