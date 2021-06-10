#include "global.h"
#include "sd.h"
#include "rtc.h"
#include "wifi.h"
#include "data.h"
#include "routes.h"
#include "mem.h"
#include "lcd.h"
#include "temperature.h"
/* #include "deep_sleep.h" */

#define RREF 430.0

#define RNOMINAL 100.0

float temperature;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");
  thermo.begin(MAX31865_3WIRE);
  EEPROM.begin(512);
  lcdSetup();
  uint8_t err = 0;
  err += rtcBegin();
  err += sdBegin();
  checkWifi(1);
  Serial.println();
  if(err) {
    Serial.println("There was an error");
    /* while(1); */
  }
  server.begin();
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);
  checkEEPROM();
  Serial.println("=====EEPROM VALUES======");
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
  Serial.println("=======================");
  Serial.println();
  Serial.println(WiFi.localIP());

  if(!MDNS.begin("esp")) {
     Serial.println("Error starting mDNS");
     return;
  }
  /* esp_sleep_enable_timer_wakeup(TS * uS_to_S); */
  /* Serial.println("Setup ESP32 to sleep for every " + String(TS) + */
  /* " Seconds"); */
  /* esp_deep_sleep_start(); */
  
}

void loop() {
  server.handleClient();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  uint16_t rtd = thermo.readRTD();
  checkWifi(0);

  Serial.println();
  Serial.println(thermo.temperature(RNOMINAL, RREF));
  temperature = thermo.temperature(RNOMINAL, RREF);
  displayUpdate(temperature);

  checkFault();
  Serial.println();
  DateTime n = getTime();
  Serial.print(n.unixtime());
  Serial.println();
  storeData(n.unixtime(), temperature);
  delay(TS * 1000);
}
