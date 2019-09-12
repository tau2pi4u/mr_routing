#include "keys.hpp"

char GetCharMapping(unsigned int x, unsigned int y)
{
    static char charMapping[4][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
    };
    return charMapping[x][y];
}

char ScanKeyboard(char * inputPins, char * outputPins)
{
    for(unsigned int outputIdx = 0; outputIdx < 3; outputIdx++)
    {
        char outputPin = outputPins[outputIdx];
        if(outputPin == 15)
        {
            for(unsigned int i = 0; i < 4; i++)
            {
                digitalWrite(inputPins[i], LOW);
            }
            for(unsigned int i = 0; i < 4; i++)
            {
                digitalWrite(inputPins[i], HIGH);
                if(digitalRead(outputPin))
                {
                    return GetCharMapping(i, outputIdx);
                }
                digitalWrite(inputPins[i], LOW);
            }
        }
        else
        {
            for(unsigned int i = 0; i < 4; i++)
            {
                digitalWrite(inputPins[i], HIGH);
            }
            for(unsigned int i = 0; i < 4; i++)
            {
                digitalWrite(inputPins[i], LOW);
                if(!digitalRead(outputPin))
                {
                    return GetCharMapping(i, outputIdx);
                }
                digitalWrite(inputPins[i], HIGH);
            }
        }
    }
    return '\0';
}

char ReadKeyboard(char * inputPins, char * outputPins)
{
    static char lastKey = '\0';
    static unsigned long long nextKeyAllowed = 0;
    char key = ScanKeyboard(inputPins, outputPins);
    if(key != lastKey && key != 0 && 
        millis() > nextKeyAllowed)
    {
        DebugPrintf("Detected key %c\n", key);
        nextKeyAllowed = millis() + 500;
        lastKey = key;
        return key;
    }
    lastKey = key;
    return '\0';
}