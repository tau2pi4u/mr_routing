#include "utils.hpp"
#include "node.hpp"
#include "api.hpp"
#include "error.hpp"
#include "gen.hpp"
#include "lcd.hpp"
#include "keys.hpp"
#include "states.hpp"

//Network variables - definitions should be in gen.hpp
const char * ssid = NETWORK;
const char * password = PASSWORD; 
const char * host = SERVER_IP;

char controlPins[] = {16, 0, 2, 14};
char readPins[] = {12, 13, 15};

char destId[6] = {'N', 0, 0, 0, 0, 0};
char trainId[5] = {0, 0, 0, 0, 0};

MainNode mainNode;

WiFiClient client;

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

void setup() {
    Serial.begin(115200);
    unsigned int x = 0;
    unsigned int y = 0;
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0,0);
    DebugPrintf("\n\nConnecting to %s\n", ssid);
    LineSafePrint(lcd, "Connecting to ", x, y);
    x = 0;
    y = 1;
    LineSafePrint(lcd, ssid, x, y);
    x = 0;
    y = 2;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        DebugPrint(".");
        LineSafePrintSplitText(lcd, ".", x, y);
    }
    DebugPrint("\nIP: ");
    DebugPrintln(WiFi.localIP());
    DebugPrint("MAC Address: ");
    DebugPrintln(WiFi.macAddress());
    GetConfig(client, mainNode, host, lcd);
    for(unsigned int i = 0; i < 4; i++)
    {
        pinMode(controlPins[i], OUTPUT);
        digitalWrite(controlPins[i], LOW);
        if(i < 3)
        {
            pinMode(readPins[i], INPUT_PULLUP);
        }
    }
}

void loop() 
{
    static unsigned int state = STATE_DISPLAY;
    static unsigned int lastState = state;
    char key = ReadKeyboard(controlPins, readPins);
    if(key == '#')
    {
        lastState = state;
        state++;
        return;
    }
    else if(key == '*')
    {
        if(state != STATE_DISPLAY)
        {
            lastState = state;
            state--;
        }
        return;
    }    
    if(state == STATE_DISPLAY)
    {
        static unsigned long long nextGet = 0;
        if(millis() > nextGet || lastState == STATE_SUBMIT)
        {
            GetConfig(client, mainNode, host, lcd);
            nextGet = millis() + 2500;
            DisplayCurrentLineStates(lcd, mainNode);
            if(mainNode.id == NULL)
            {
                unsigned int x = 0; 
                unsigned int y = 1;
                static char buf[64];
                sprintf(buf, "MAC: %s", WiFi.macAddress().c_str());
                LineSafePrintSplitText(lcd, buf, x, y);
                nextGet += 7500; // give time to read address
            }
        }
    }
    else if (state == STATE_INPUT_DEST)
    {
        static unsigned long long nextDisp = 0;
        static unsigned int idx = 1;
        if(lastState != state)
        { 
            idx = 1; 
            memset(&destId[1], 0, 5 * sizeof(char));
        }
        if(key != 0)
        {
            if(idx < 5)
            {
                destId[idx++] = key;
            }
        }
        if(millis() > nextDisp || key != 0)
        {
            DisplayCurrentText(lcd, "Destination", destId, 5);
            nextDisp = millis() + 2500;
        }
    }
    else if (state == STATE_INPUT_ID)
    {
        static unsigned long long nextDisp = 0;
        static unsigned int idx = 0;
        if(lastState != state)
        { 
            idx = 0; 
            memset(trainId, 0, 5 * sizeof(char));
        }
        if(key != 0)
        {
            if(idx < 4)
            {
                trainId[idx++] = key;
            }
        }
        if(millis() > nextDisp || key != 0)
        {
            DisplayCurrentText(lcd, "Train ID", trainId, 4);
            nextDisp = millis() + 2500;
        }
    }
    else if(state == STATE_SUBMIT)
    {
        if(destId[1] == 0)
        {
            AddTrainToLine(client, host, mainNode, "remove_train", trainId, lcd);
        }
        else
        {
            AddTrainToLine(client, host, mainNode, destId, trainId, lcd);
        }
        state = STATE_DISPLAY;
    }
    lastState = state;
}