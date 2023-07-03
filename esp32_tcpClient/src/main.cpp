// sources: https://randomnerdtutorials.com/esp32-client-server-wi-fi/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "credentials.h"

// put function declarations here:

// connect to wifi
void startWifi();

// request and receive data from server
uint8_t* getData();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  startWifi();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED ){
    uint8_t * data =  getData();
  }
}

// put function definitions here:

void startWifi(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting…");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}