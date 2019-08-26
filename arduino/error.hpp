#pragma once

#include "utils.hpp"

static const int ERROR_COUNT = 10;
static const int EMPTY_RESPONSE_INDEX = 4;

static const char * ERROR_0 PROGMEM = {0};
static const char * ERROR_1 PROGMEM = "Line busy";
static const char * ERROR_2 PROGMEM = "Unknown node";
static const char * ERROR_3 PROGMEM = "Unimplemented";
static const char * ERROR_4 PROGMEM = "Empty response";
static const char * ERROR_5 PROGMEM = "No sender id";
static const char * ERROR_6 PROGMEM = "No train id";
static const char * ERROR_7 PROGMEM = "Node does not exist";
static const char * ERROR_8 PROGMEM = "No such connection exists";
static const char * ERROR_9 PROGMEM = "Unknown error";

static const char * ERROR_MESSAGE_TEXT [ERROR_COUNT] PROGMEM =
{
    ERROR_0,
    ERROR_1,
    ERROR_2,
    ERROR_3,
    ERROR_4,
    ERROR_5,
    ERROR_6,
    ERROR_7,
    ERROR_8,
    ERROR_9  
};

bool IsEmpty(const String & errorStr);
bool IsError(const String & errorStr);
const char * GetErrorMessageFromReturnString(const char * error);
const char * GetErrorMessage(unsigned int errorCode);