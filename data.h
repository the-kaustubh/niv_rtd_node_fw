#ifndef _DATA_H
#define _DATA_H

#include "global.h"

uint8_t storeData(uint32_t ts, float temperature) {
  if(WiFi.status() == WL_CONNECTED) {
    char postdata[100];
    HTTPClient http;
    http.begin(PROT  HOST  ENDPOINT);
    Serial.println(PROT HOST ENDPOINT);
    http.addHeader("Content-Type", "application/json");

/*     sprintf(postdata, */ 
/*         "uid=%s&" */
/*         "temperature=%0.2f&" */
/*         "user=%s&" */
/*         "humidity=0&" */
/*         "co2=0&" */
/*         "pressure=0", */
/*         UID, */
/*         temperature, */
/*         USER */
/*         ); */

    sprintf(postdata, 
        "{\"uid\":\"%s\","
        "\"temperature\":\"%0.2f\","
        "\"user\":\"%s\","
        "\"humidity\":\"0\","
        "\"co2\":\"0\","
        "\"pressure\":\"0\"}",
        UID,
        temperature,
        USER
        );

    Serial.println(postdata);

    int resp = http.POST(postdata);
    Serial.println(http.getString());
    http.end();
    return resp;
  } else {
    writeReading(ts, temperature);
    return 1;
  }
}

#endif
