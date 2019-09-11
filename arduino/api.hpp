#pragma once

#include "utils.hpp"
#include "node.hpp"
#include "error.hpp"
#include "lcd.hpp"

bool EstablishClientConnection(WiFiClient & client, const char * host);
void GetConfig(WiFiClient & client, MainNode & mainNode, const char * host, LiquidCrystal_I2C & lcd);
void AddTrainToLine(WiFiClient & client, const char * host, MainNode & mainNode, const char * nodeId, const char * trainId, LiquidCrystal_I2C & lcd);
