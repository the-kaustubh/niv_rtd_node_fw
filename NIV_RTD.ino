#include "global.h"
#include "sd.h"
#include "rtc.h"
#include "wifi.h"
#include "data.h"

Adafruit_MAX31865 thermo = Adafruit_MAX31865(27, 14, 12, 13);

#define RREF 430.0

#define RNOMINAL 100.0

float temperature;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

  thermo.begin(MAX31865_3WIRE);
  uint8_t err;
  err = rtcBegin();
  err += sdBegin();
  if(checkWifi()) {
    Serial.println("Wifi Connected.");
  }
  if(err) {
  while(1);
  }
}

void loop() {
  uint16_t rtd = thermo.readRTD();

  Serial.println();
  Serial.println(thermo.temperature(RNOMINAL, RREF));
  temperature =thermo.temperature(RNOMINAL, RREF);

  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    thermo.clearFault();
  }
  Serial.println();
  DateTime n = getTime();
  Serial.print(n.unixtime());
  Serial.println();
  storeData(n.unixtime(), temperature);
  delay(1000);
}
