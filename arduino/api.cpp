#include "api.hpp"

bool EstablishClientConnection(WiFiClient & client, const char * host)
{
    client.connect(host, 5000);
    if(!client.connected())
    {
        DebugPrintln("Client not connected");
        client.stop();
        return false;
    }
    return true;
}

bool EstablishHTTPConnection(WiFiClient & client, HTTPClient & http, const char * host, const String & uri)
{
    String hostUrl = "http://";
    hostUrl += host;
    DebugPrint("url: ");
    DebugPrintln(hostUrl + ":5000" + uri);
    http.begin(client, hostUrl, 5000, uri);
    if(!http.connected())
    {
        DebugPrintln("Not connected");
        return false;
    }
    return true;
}

bool SendPUTRequest(HTTPClient & http, const String & request, LiquidCrystal_I2C & lcd)
{
    http.addHeader("Content-Type", "application/json");
    DebugPrintf("Request: %s\n", request.c_str());
    int responseCode = http.PUT(request);
    if(responseCode > 0)
    {
        DebugPrint("Response code: ");
        DebugPrintln(String(responseCode));
        DebugPrint("Response: ");
        String response = http.getString();
        DebugPrintln(response);
        if(IsError(response))
        {
            static char buf[64];
            for(unsigned int i = 0; i < response.length(); i++)
            {
                buf[i] = response[i];
            }
            buf[response.length()] = 0;
            DebugPrintf("Buf: %s\n", buf);
            const char * error = GetErrorMessageFromReturnString(buf);
            DebugPrint("Response is now: ");
            DebugPrintln(response);
            DebugPrintf("Error: %s\n", error);
            DisplayErrorMessage(lcd, error);
            return false;
        }
        return true;
    }
    else
    {
        DebugPrintln("GetConfig: error");
        DebugPrintln(String(responseCode));
        DebugPrintln(http.getString());
        return false;
    }
}

void GetConfig(WiFiClient & client, MainNode & mainNode, const char * host, LiquidCrystal_I2C & lcd)
{
    if(!EstablishClientConnection(client, host))
    { 
        DisplayErrorMessage(lcd, "Could not connect to server");
        return; 
    }
    HTTPClient http;
    String uri = "/api/node/new_node";
    if(!EstablishHTTPConnection(client, http, host, uri))
    { 
        DisplayErrorMessage(lcd, "Could establish HTTP connection");
        return; 
    }    
    String request = "{\"mac_address\" : \"" + WiFi.macAddress() + "\"}";
    if(!SendPUTRequest(http, request, lcd)){ return; }   
    GetMainNodeFromConfig(mainNode, http.getString());
    http.end();
    client.stop();
}

void AddTrainToLine(WiFiClient & client, const char * host, MainNode & mainNode, const char * nodeId, const char * trainId, LiquidCrystal_I2C & lcd)
{
    if(!EstablishClientConnection(client, host)){ return; }
    HTTPClient http;
    String uri = "/api/node/";
    uri += nodeId;
    if(!EstablishHTTPConnection(client, http, host, uri)){ return; }    
    String request = "{\"sender_id\" : \"" + String(mainNode.id) + 
                     "\", \"train_id\" : \"" + String(trainId) + "\"}";
    if(!SendPUTRequest(http, request, lcd)){ return; }   
    DebugPrint(http.getString());
    http.end();
    client.stop();
    GetConfig(client, mainNode, host, lcd);
}