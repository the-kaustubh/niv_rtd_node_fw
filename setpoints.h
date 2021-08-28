#ifndef _SETPT_H
#define _SETPT_H

#include "global.h"

void fetchSetpoint() {
  HTTPClient http;

  String url = PROT + HOST + "/write/setpoints/" + UID;
  http.begin(url);
  http.addHeader("Accept", "application/json");

  Serial.println(url);
  int resp = http.GET();
  String response = http.getString();
  Serial.println(response);

  DynamicJsonDocument json(1024);
  deserializeJson(json, response);
  Serial.println("+++++++++JSON DATA++++++++++");
  Serial.println(json['temperaturemin'] + "," + json['temperaturemax']);
  Serial.println(json['humiditymin'] + "," + json['humiditymax']);
  Serial.println(json['co2min'] + "," + json['co2max']);
  Serial.println("++++++++++++++++++++++++++++");

  updateSetPoint(TEMP_MIN_ADDR, json['temperaturemin'], json['temperaturemax']);
  updateSetPoint(HUM_MIN_ADDR, json['humiditymin'], json['humiditymax']);
  updateSetPoint(CO2_MIN_ADDR, json['co2min'], json['co2max']);

  http.end();
}

#endif
