#pragma once

#include "utils.hpp"

static const int ERROR_COUNT = 10;
static const int EMPTY_RESPONSE_INDEX = 4;

// Needs to fit into a buffer of 64 chars
static const char * ERROR_0 = {0};
static const char * ERROR_1 = "Line busy";
static const char * ERROR_2 = "Unknown node";
static const char * ERROR_3 = "Unimplemented";
static const char * ERROR_4 = "Empty response";
static const char * ERROR_5 = "No sender id";
static const char * ERROR_6 = "No train id";
static const char * ERROR_7 = "Node does not exist";
static const char * ERROR_8 = "No such connection exists";
static const char * ERROR_9 = "Unknown error";

static const char * ERROR_MESSAGE_TEXT [ERROR_COUNT] =
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

bool IsEmpty(String errorStr);
bool IsError(String errorStr);
const char * GetErrorMessageFromReturnString(const char * error);
const char * GetErrorMessage(unsigned int errorCode);