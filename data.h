#ifndef _DATA_H
#define _DATA_H

uint8_t storeData(uint32_t ts, float temperature) {
  if(WiFi.status() == WL_CONNECTED) {
    char postdata[100];
    HTTPClient http;
    http.begin(PROT  HOST  ENDPOINT);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    sprintf(postdata, 
        "uid=%s&"
        "temperature=%0.2f&"
        "user=%s",
        "humidity=0&"
        "co2=0&"
        "pressure=0",
        UID,
        temperature,
        USER
        );

    Serial.println(postdata);

    int resp = http.POST(postdata);
    http.end();
    return resp;
  } else {
    writeReading(ts, temperature);
    return 1;
  }
}

#endif
