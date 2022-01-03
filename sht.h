#ifndef _DHT_H
#define _DHT_H

#include "global.h"
#include <Wire.h>
#include "Adafruit_SHT31.h"

void DHT_init() {
  //dht.begin(DHTPIN, DHTTYPE);
  //dht.begin();
  Serial.println("SHT Started");
  sht31.begin(0x44);
}

int getValuesDHT(float * t, float * h) {
  //float _h = dht.readHumidity();
 // float _t = dht.readTemperature();
  float _t = sht31.readTemperature();
  float _h = sht31.readHumidity();
  if(isnan(_h) || isnan(_t)) {
    Serial.println("Failed to read from SHT Sensor");
    return 0;
  }
  *h = _h;
  *t = _t;
  return 1;
}

#endif /* ifndef _DHT_H */
