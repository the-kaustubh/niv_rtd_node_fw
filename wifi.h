#ifndef _WIFI_A_H
#define _WIFI_A_H

#define SSID ("Galaxy M514B85")
#define PASS ("pzzk3333")

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

int checkWifi() {
  if(WiFi.status() != WL_CONNECTED) {
    // TODO: Make dynamic
    WiFi.begin(SSID, PASS);
    int times = 0;
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      times ++;
      delay(500);
      if(times > 20) {
        return 0;
      }

    }
    Serial.println();
    return 1;
  }
  return 1;
}

#endif
