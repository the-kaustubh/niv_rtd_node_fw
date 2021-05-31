#ifndef _WIFI_A_H
#define _WIFI_A_H

#include "global.h"

void checkWifi(int retry) {
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SSID.c_str(), PASS.c_str());
    int times = 0;
    if(retry) {
      while(WiFi.status() != WL_CONNECTED) {
        WiFi.begin(SSID.c_str(), PASS.c_str());
        Serial.print(".");
        times ++;
        delay(500);
        if(times > 20) {
          Serial.print("Couldn't Connect to Network");
          return;
        }
      }
    }
    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("Trying default Network");
      WiFi.begin(DEF_SSID, DEF_PASS);
      while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
      }
    }
    Serial.println();
    delay(1000);
    if(WiFi.status() == WL_CONNECTED) {
      Serial.print("Connected to Network: ");
      Serial.println(SSID);
    }
    Serial.println();
  }
}

#endif
