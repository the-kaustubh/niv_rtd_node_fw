#ifndef _DATA_H
#define _DATA_H

#include "global.h"

int postRequestWithTS(float t, uint32_t ts) {

  char postdata[200];
  HTTPClient http;
  http.begin(PROT + HOST + ENDPOINT);
  Serial.println(PROT + HOST + ENDPOINT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");

  snprintf(postdata, 200,
      "{"
      "\"uid\":\"%s\","
      "\"temperature\":\"%0.2f\","
      "\"user\":\"%s\","
      "\"humidity\":\"0\","
      "\"co2\":\"0\","
      "\"pressure\":\"0\","
      "\"backup\": \"1\","
      "\"datetime\": \"%ld\""
      "}",
      UID,
      t,
      USER,
      ts
      );

  Serial.println(postdata);

  int resp = http.POST(postdata);
  Serial.println(resp);
  Serial.println(http.getString());
  http.end();
  return resp;
}

int postRequest(float t) {

  char postdata[100];
  HTTPClient http;
  http.begin(PROT + HOST + ENDPOINT);
  Serial.println(PROT + HOST + ENDPOINT);
  http.addHeader("Content-Type", "application/json");

  sprintf(postdata, 
      "{\"uid\":\"%s\","
      "\"temperature\":\"%0.2f\","
      "\"user\":\"%s\","
      "\"humidity\":\"0\","
      "\"co2\":\"0\","
      "\"pressure\":\"0\"}",
      UID,
      t,
      USER
      );

  Serial.println(postdata);

  int resp = http.POST(postdata);
  Serial.println(http.getString());
  http.end();
  return resp;
}

uint8_t storeData(uint32_t ts, float temperature) {
  if(WiFi.status() == WL_CONNECTED) {
    if(readingsPresent()) {
      Serial.println("Readings Present");
      File ts = SD.open(FILE_SAVE, FILE_READ);
      String fmt = "";
      if(!ts) {
        Serial.println("File not found.");
        return 0;
      }
      while(ts.available()) {
        Serial.println(ts.available());
        String f = ts.readStringUntil('\n');
        if(f.length() < 4) {
          Serial.println("Readings Empty");
          break;
        }
        uint32_t tstamp;
        float tem;
        sscanf(f.c_str(), "%ld,%f", &tstamp, &tem);
        Serial.print("Tstamp");
        Serial.println(tstamp);
        Serial.print("Temp");
        Serial.println(tem);
        postRequestWithTS(tem, tstamp);
      }
      ts.close();
      clearFile(FILE_SAVE);
      Serial.println("Sent SD card data");

    } else {
      int resp = postRequest(temperature);
      Serial.println("Sent live data");
      return resp;
    }
  } else {
    writeReading(ts, temperature);
      Serial.println("Saved data to SD card");
    return 1;
  }
}

#endif
