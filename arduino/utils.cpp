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
    static char buf[256];
    va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    Serial.printf("%s", buf);
    va_end(args);
    #endif
}

void DebugPrintln(const String & str)
{
    #ifdef _DEBUG
    Serial.println(str);
    #endif
}

void DebugPrintln(const IPAddress & ip)
{
    #ifdef _DEBUG
    Serial.println(ip);
    #endif
}

void DebugPrint(const String & str)
{
    #ifdef _DEBUG
    Serial.print(str);
    #endif
}