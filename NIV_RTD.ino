#include "global.h"
#include "sd.h"
#include "rtc.h"
#include "wifi.h"
#include "data.h"
#include "routes.h"
#include "mem.h"
#include "lcd.h"
#include "temperature.h"
#include "ndir_co2.h"
/* #include "deep_sleep.h" */

#define DEBUG
#define RREF 430.0

#define RNOMINAL 100.0

float temperature, co2;
uint32_t rtd;
uint8_t BUZZER_PIN = 27;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");
  thermo.begin(MAX31865_3WIRE);
  EEPROM.begin(512);
  lcdSetup();
  uint8_t err = 0;
  err += rtcBegin();
  err += sdBegin();
  err += !co2Sensor.begin();
  checkWifi(1);
  Serial.println();
  if(err) {
    Serial.println("There was an error");
    #ifndef DEBUG
    while(1);
    #endif
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
  delay(20000);
  uint16_t rtd = thermo.readRTD();

}

void loop() {
  server.handleClient();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  checkWifi(0);

  Serial.println();
  #ifdef DEBUG
  temperature = 30.0;
  #else
  temperature = thermo.temperature(RNOMINAL, RREF);
  #endif

  if(co2Sensor.measure()) {
    co2 = co2Sensor.ppm;
  } else {
    Serial.println("Error: Sensor Communication Error in CO2");
  }

  Serial.println(temperature);
  displayUpdate(temperature);

  checkFault();
  Serial.println();

  DateTime n = getTime();
  uint32_t ts = 0;
  #ifdef DEBUG
  ts = 1623718150;
  #else
  ts = n.unixtime();
  #endif

  storeData(ts, temperature, co2);
  delay(TS * 1000);
}
