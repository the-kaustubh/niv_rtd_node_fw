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
  String ddata;
  ddata = json['temperaturemin'].as<const char*>();
  ddata += ", ";
  ddata += json['temperaturemax'].as<const char*>();
  ddata += "\n";
  ddata += json['humiditymin'].as<const char*>();
  ddata += ", ";
  ddata += json['humiditymax'].as<const char*>();
  ddata += "\n";
  ddata += json['co2min'].as<const char*>();
  ddata += ", ";
  ddata += json['co2max'].as<const char*>();
  ddata += "\n";

  Serial.println("+++++++++JSON DATA++++++++++");
  Serial.println(ddata);
  Serial.println("++++++++++++++++++++++++++++");

  updateSetPoint(TEMP_MIN_ADDR, json['temperaturemin'], json['temperaturemax']);
  updateSetPoint(HUM_MIN_ADDR, json['humiditymin'], json['humiditymax']);
  updateSetPoint(CO2_MIN_ADDR, json['co2min'], json['co2max']);

  http.end();
}

#endif
