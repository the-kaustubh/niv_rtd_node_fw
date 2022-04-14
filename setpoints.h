#ifndef _SETPT_H
#define _SETPT_H

#include "global.h"

float * fetchSetpoint(void) {
  HTTPClient http;

  String url = PROT + HOST + "/server/write/setpoints/" + UID;
  http.begin(url);
  http.addHeader("Accept", "application/json");

  Serial.println(url);
  int resp = http.GET();
  String response = http.getString();
  Serial.println(response);

  DynamicJsonDocument json(1024);
  deserializeJson(json, response);
  http.end();


#ifdef RTD_NODE
  float TEMP_MIN = json["temperaturemin"];
  float TEMP_MAX = json["temperaturemax"];
#endif

#ifdef DHT_NODE
  float TEMP_MIN = json["temperaturemin"];
  float TEMP_MAX = json["temperaturemax"];
  float HUM_MIN = json["humiditymin"];
  float HUM_MAX = json["humiditymax"];
#endif

#ifdef CO2_NODE
  float CO2_MIN = json["co2min"];
  float CO2_MAX = json["co2max"];
#endif

#ifdef MQ_NODE
  float CO_MIN = json["temperaturemax"];
  float CO_MAX = json["temperaturemin"];

  float NO_MIN = json["humiditymin"];
  float NO_MAX = json["humiditymax"];

  float H2S_MIN = json["co2min"];
  float H2S_MAX = json["co2max"];
#endif

#ifdef RTD_NODE
  updateSetPoint(TEMP_MIN_ADDR, json['temperaturemin'], json['temperaturemax']);
#endif
#ifdef DHT_NODE
  updateSetPoint(TEMP_MIN_ADDR, json['temperaturemin'], json['temperaturemax']);
  updateSetPoint(HUM_MIN_ADDR, json['humiditymin'], json['humiditymax']);
#endif
#ifdef CO2_NODE
  updateSetPoint(CO2_MIN_ADDR, json['co2min'], json['co2max']);
#endif
#ifdef MQ_NODE
  updateSetPoint(TEMP_MIN_ADDR, json['temperaturemin'], json['temperaturemax']);
  updateSetPoint(HUM_MIN_ADDR, json['humiditymin'], json['humiditymax']);
  updateSetPoint(CO2_MIN_ADDR, json['co2min'], json['co2max']);
#endif
  // updateSetPoint(TEMP_MIN_ADDR,20,30);

  static float setvalues[6];
  setvalues[0] = TEMP_MIN;
  setvalues[1] = TEMP_MAX;
  setvalues[2] = HUM_MIN;
  setvalues[3] = HUM_MAX;
  setvalues[4] = CO2_MIN;
  setvalues[5] = CO2_MAX;
  return setvalues;
}

#endif
