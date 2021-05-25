#include "global.h"
#include "sd.h"
#include "rtc.h"
#include "wifi.h"
#include "data.h"
#include "routes.h"
#include "mem.h"

Adafruit_MAX31865 thermo = Adafruit_MAX31865(27, 14, 12, 13);

#define RREF 430.0

#define RNOMINAL 100.0

float temperature;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

  thermo.begin(MAX31865_3WIRE);
  EEPROM.begin(512);
  uint8_t err = 0;
  /* err += rtcBegin(); */
  err += sdBegin();
  checkWifi(1);
  if(err) {
    while(1);
    Serial.println("There was an error");
  }
  server.begin();
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);
  Serial.println(WiFi.localIP());
  checkEEPROM();
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("PASS: ");
  Serial.println(PASS);
  Serial.print("UID: ");
  Serial.println(UID);
  Serial.print("USER: ");
  Serial.println(USER);
  Serial.print("HOST: ");
  Serial.println(HOST);
  Serial.println("===========");
}

void loop() {
  server.handleClient();
  checkWifi(0);
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
