#include "utils.hpp"
#include "node.hpp"
#include "api.hpp"
#include "error.hpp"
#include "gen.hpp"

// Network variables - definitions should be in gen.hpp
const char * ssid = NETWORK;
const char * password = PASSWORD; 
const char * host = SERVER_IP;

MainNode mainNode;

WiFiClient client;

void setup() {
    Serial.begin(115200);
    delay(10); 
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid); 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    GetConfig(client, mainNode, host);
}

void loop() 
{
    GetConfig(client, mainNode, host);
    AddTrainToLine(client, host, mainNode, "A1", "1014");
    delay(100000);
}