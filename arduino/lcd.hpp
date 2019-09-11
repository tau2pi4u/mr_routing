#pragma once
#include "utils.hpp"
#include "node.hpp"

#define LCD_WIDTH 20
#define LCD_HEIGHT 4

void LineSafePrint(LiquidCrystal_I2C & lcd, const char * text, unsigned int & x, unsigned int & y);
void LineSafePrint(LiquidCrystal_I2C & lcd, const char * text, const char * altText, unsigned int & x, unsigned int & y);
void LineSafePrintSplitText(LiquidCrystal_I2C & lcd, char * text, unsigned int & x, unsigned int & y);
void DisplayCurrentLineStates(LiquidCrystal_I2C & lcd, MainNode & node);
void DisplayErrorMessage(LiquidCrystal_I2C & lcd, const char * error);
void DisplayCurrentText(LiquidCrystal_I2C & lcd, const char * title, const char * text, unsigned int maxChar);