#pragma once

#define _DEBUG

#include <Arduino.h>
#include "ESP8266WiFi.h" 
#include "ESP8266HTTPClient.h"
#include <LiquidCrystal_I2C.h>

void RemoveCharacterFromString(String & str, char toRemove);
void DebugPrintf(const char * format, ...);
void DebugPrintln(const String & str);
void DebugPrintln(const IPAddress & ip);
void DebugPrint(const String & str);