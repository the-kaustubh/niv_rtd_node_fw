#ifndef _WIFI_A_H
#define _WIFI_A_H

#define SSID ("Galaxy M514B85")
#define PASS ("pzzk3333")

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

uint8_t isConnected = false;

void checkWifi() {
  if(WiFi.status() != WL_CONNECTED) {
    // TODO: Make dynamic
    WiFi.begin(SSID, PASS);
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
    }
    Serial.println();
  }
}

#endif
