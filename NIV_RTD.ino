#include <Adafruit_MAX31865.h>

#include "sd.h"
#include "rtc.h"

Adafruit_MAX31865 thermo = Adafruit_MAX31865(27, 14, 12, 13);

#define RREF 430.0

#define RNOMINAL 100.0

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

  thermo.begin(MAX31865_3WIRE);
  rtcBegin();
  /* sdBegin(); */
}

void loop() {
  uint16_t rtd = thermo.readRTD();

  Serial.println();
  Serial.println(thermo.temperature(RNOMINAL, RREF));

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
  Serial.print(n.hour());
  Serial.print(":");
  Serial.print(n.minute());
  Serial.print(":");
  Serial.print(n.second());
  Serial.println();
  Serial.print("unixtime: ");
  Serial.print(n.unixtime());
  Serial.println();
  delay(1000);
}
