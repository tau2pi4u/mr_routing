#include "lcd.hpp"

void LineSafePrint(LiquidCrystal_I2C & lcd, const char * text, unsigned int & x, unsigned int & y)
{
    unsigned int textLen = strlen(text);
    if(x + textLen <= LCD_WIDTH)
    {
        lcd.setCursor(x, y);
        lcd.print(text);
        x += textLen;
    }
    else
    {
        x = 0;
        y++;
        if(y >= LCD_HEIGHT)
        {
            return;
        }
        lcd.setCursor(x, y);
        lcd.print(text);
    }    
}

void LineSafePrint(LiquidCrystal_I2C & lcd, const char * text, const char * altText, unsigned int & x, unsigned int & y, bool altNewLine)
{
    unsigned int textLen = strlen(text);
    if(x + textLen <= LCD_WIDTH)
    {
        lcd.setCursor(x, y);
        lcd.print(text);
        x += textLen;
    }
    else
    {
        if(altNewLine)
        {
            y++;
            x = 0;
            if(y >= LCD_HEIGHT)
            {
                return;
            }
        }
        unsigned int altTextLen = strlen(altText);
        if(x + altTextLen <= LCD_WIDTH)
        {
            lcd.setCursor(x, y);
            lcd.print(altText);
            x += altTextLen;
        }
        else
        {        
            x = 0;
            y++;
            if(y >= LCD_HEIGHT)
            {
                return;
            }
            lcd.setCursor(x, y);
            lcd.print(text);
            x += textLen;
        }
    }
}

void LineSafePrintSplitText(LiquidCrystal_I2C & lcd, char * text, unsigned int & x, unsigned int & y)
{
    unsigned int remainingText = strlen(text);
    char * tPos = text;
    static char buf[32];
    while(remainingText > 0)
    {
        unsigned int remainingX = LCD_WIDTH - x;
        if(remainingX >= remainingText)
        {
            DebugPrintf("Fitting characters (%s) into remaining space at (%u, %u)\n", tPos, x, y);
            LineSafePrint(lcd, tPos, x, y);
            break;
        }
        else
        {
            bool bSuccess = false;
            for(char * p = tPos + remainingX + 1; p != tPos; p--)
            {
                if(*p == ' ')
                {
                    strncpy(buf, tPos, p - tPos); // no need to include the space
                    buf[p-tPos] = 0;
                    DebugPrintf("Buf = %s\n", buf);
                    DebugPrintf("Copied %u characters\n", p - tPos);
                    LineSafePrint(lcd, buf, x, y);
                    remainingText -= p - tPos + 1;
                    tPos = p + 1;
                    DebugPrintf("Remaing = %s\n", tPos);
                    DebugPrintf("%u characters left\n", strlen(tPos));
                    bSuccess = true;
                    break;
                }
            }
            if(!bSuccess && x == 0)
            {
                return;
            }
            else if(!bSuccess)
            {
                DebugPrintf("Could not fit characters into remaining space %u\n", remainingX);
                x = 0;
                y++;
            }
            
        }
    }
}

void DisplayCurrentLineStates(LiquidCrystal_I2C & lcd, MainNode & mainNode)
{
    lcd.clear();
    lcd.setCursor(0,0);
    unsigned int cursorX = 0;
    unsigned int cursorY = 0;
    static char buf[64];
    if(mainNode.id == NULL)
    {
        LineSafePrint(lcd, "ID:N/A | ", cursorX, cursorY);
        return;
    }
    else
    {
        sprintf(buf, "ID:%s | ", mainNode.id);
        LineSafePrint(lcd, buf, cursorX, cursorY);
    }
    LineSafePrint(lcd, "In: ", cursorX, cursorY);
    bool first = true;
    for(unsigned int i = 0; i < mainNode.iNodeCount; i++)
    {
        if(!first)
        {
            LineSafePrint(lcd, ", ", ",", cursorX, cursorY, false);
        }
        else
        {
            first = false;
        }
        sprintf(buf, "%s:%s", mainNode.iNodes[i].id, mainNode.iNodes[i].trainId);
        LineSafePrint(lcd, buf, cursorX, cursorY);
    }
    LineSafePrint(lcd, " | Out: ", "Out: ", cursorX, cursorY);
    first = true;
    for(unsigned int i = 0; i < mainNode.oNodeCount; i++)
    {
        if(!first)
        {
            LineSafePrint(lcd, ", ", ",", cursorX, cursorY, false);
        }
        else
        {
            first = false;
        }
        sprintf(buf, "%s:%s", mainNode.oNodes[i].id, mainNode.oNodes[i].trainId);
        LineSafePrint(lcd, buf, cursorX, cursorY);
    }
}

void DisplayErrorMessage(LiquidCrystal_I2C & lcd, const char * error)
{
    lcd.clear();
    unsigned int x = 0;
    unsigned int y = 0;
    static char buf[64];
    sprintf(buf, "Error: %s", error);
    lcd.setCursor(x, y);
    LineSafePrintSplitText(lcd, buf, x, y);
    delay(2500);
}

void DisplayCurrentText(LiquidCrystal_I2C & lcd, const char * title, const char * text, unsigned int maxChar)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(title);
    lcd.setCursor(0, 1);
    lcd.print(text);
    lcd.setCursor(0, 2);
    lcd.printf("%u/%u", strlen(text), maxChar);
}