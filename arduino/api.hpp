#pragma once

#include "utils.hpp"
#include "node.hpp"

bool EstablishClientConnection(WiFiClient & client, const char * host);
void GetConfig(WiFiClient & client, MainNode & mainNode, const char * host);
void AddTrainToLine(WiFiClient & client, const char * host, MainNode & mainNode, const char * nodeId, const char * trainId);
