#include "global.h"
#include "sd.h"
#include "rtc.h"
#include "wifi.h"
#include "data.h"
#include "routes.h"
#include "mem.h"
#include "lcd.h"
/* #include "deep_sleep.h" */

#define RTD_NODE
/* #define DHT_NODE */
/* #define CO2_NODE */


#ifdef CO2_NODE
#include "ndir_co2.h"
#endif

#ifdef DHT_NODE
#include "dht.h"
float dht_temp, dht_hum;
#endif

#ifdef RTD_NODE
#include "temperature.h"
#define RREF 430.0
#define RNOMINAL 100.0
uint32_t rtd;
float temperature, co2;
#endif

#define DEBUG

#define BUZZER_PIN (27)

void setup() {
  Serial.begin(115200);
#ifdef RTD_NODE
  thermo.begin(MAX31865_3WIRE);
#endif
  EEPROM.begin(512);
  lcdSetup();
  uint8_t err = 0;

  err += rtcBegin();
  err += sdBegin();

#ifdef CO2_NODE
  err += !co2Sensor.begin();
#endif

  checkWifi(1);
  Serial.println();
  if(err) {
    Serial.println("There was an error");
#ifndef DEBUG
    while(1);
#endif
  }

#ifdef DHT_NODE
  DHT_init();
#endif

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
#ifdef RTD_NODE
  rtd = thermo.readRTD();
#endif

}

void loop() {
  server.handleClient();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  checkWifi(0);

  Serial.println();
#ifdef RTD_NODE
#ifdef DEBUG
  temperature = 30.0;
#else
  temperature = thermo.temperature(RNOMINAL, RREF);
#endif
#endif

#ifdef CO2_NODE
  if(co2Sensor.measure()) {
    co2 = co2Sensor.ppm;
  } else {
    Serial.println("Error: Sensor Communication Error in CO2");
  }
#endif

#ifdef RTD_NODE
  Serial.println(temperature);
  displayUpdate(temperature);

  checkFault();
  Serial.println();
#endif

#ifdef DHT_NODE
  getValuesDHT(&dht_temp, &dht_hum);
#endif

  DateTime n = getTime();
  uint32_t ts = 0;
#ifdef DEBUG
  ts = 1623718150;
#else
  ts = n.unixtime();
#endif


  storeData(ts,
#ifdef DHT_NODE
      dht_temp
#else
      temperature
#endif
      ,
#ifdef CO2_NODE
      co2
#else
      0.0
#endif
      ,
#ifdef DHT_NODE
      dht_hum
#else
      0.0
#endif
      );

  /* #ifdef CO2_NODE */
  /* #ifdef DHT_NODE */
  /* storeData(ts, dht_temp, co2, dht_hum); */
  /* #else */
  /* storeData(ts, temperature, co2, 0f); */
  /* #endif */
  /* #else */
  /* #ifdef DHT_NODE */
  /* storeData(ts, dht_temp, 0, dht_hum); */
  /* #else */
  /* storeData(ts, temperature, 0, 0); */
  /* #endif */
  /* #endif */

  delay(TS * 1000);
}
