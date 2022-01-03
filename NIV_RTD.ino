#include "global.h"
#include "sd.h"
#include "rtc.h"
#include "wifi.h"
#include "data.h"
#include "routes.h"
#include "mem.h"
#include "lcd.h"
#include "battery.h"
#include "setpoints.h"
#define RTD_PIN 25
#ifdef CO2_NODE
#include "mh_z16_co2.h"
#endif

#define ABS(x) ((x<0) ? (-(x)) : (x))

#ifdef DHT_NODE
#include "sht.h"
float dht_temp, dht_hum;
#endif

#ifdef RTD_NODE
#include "temperature.h"
#define RREF 430.0
#define RNOMINAL 100.0
uint32_t rtd;
float temperature;
#endif

#ifdef CO2_NODE
float co2;
#endif

#ifdef RTD_NODE
float prev_temp = 0.0;
#define TEMP_TH (0.5)
#endif

#ifdef DHT_NODE
float prev_dht_temp = 0.0;
float prev_dht_humidity = 0.0;
#define DHT_TEMP_TH (0.5)
#define DHT_HUM_TH (0.5)
#endif

#ifdef CO2_NODE
float prev_co2 = 0.0;
#define CO2_TH (0.5)
#endif

uint8_t isThresholdExceeded = 0;
int battery = 0;
DateTime n;

int faultyFlag = 0;
uint8_t buzzerFlag = 0;
uint64_t buzzerTime;

uint64_t lastSend = 0;

void FetchAllSetpoints() {
  setvalues=fetchSetpoint();
  TEMP_MIN=setvalues[0];
  TEMP_MAX=setvalues[1];
  HUM_MIN=setvalues[2];
  HUM_MAX=setvalues[3];
  CO2_MIN=setvalues[4];
  CO2_MAX=setvalues[5];
}

void ReadAllPeripherals() {

#ifdef RTD_NODE
  temperature = thermo.temperature(RNOMINAL, RREF);
  Serial.print("RTD temperature: ");
  Serial.println(temperature);
  if(temperature > TEMP_MAX || temperature < TEMP_MIN) {
    faultyFlag += 1;
  }
  Serial.println(temperature);
  checkFault();
  Serial.println();
#endif

#ifdef CO2_NODE
  if(!co2Sensor.isWarming()) {
    co2 = co2Sensor.getPPM();
    if(co2 > CO2_MAX || co2 < CO2_MIN) {
      faultyFlag += 1;
    }
  } else {
    Serial.println("Error: Sensor Communication Error in CO2");
  }
#endif

#ifdef DHT_NODE
  getValuesDHT(&dht_temp, &dht_hum);

  if((dht_temp > TEMP_MAX) || (dht_temp < TEMP_MIN)) {
    faultyFlag += 1;
  }
  if((dht_hum > HUM_MAX) || (dht_hum < HUM_MIN)) {
    faultyFlag += 1;
  }
#endif
  Serial.println("battery:=");
  battery = getBattery();
  Serial.println(battery);
}

void displaySetpoints() {
  Serial.print("Min. Temp:=");
  Serial.println(TEMP_MIN);
  Serial.print("Max. Temp:=");
  Serial.println(TEMP_MAX);

  Serial.print("Min. hum:=");
  Serial.println(HUM_MIN);
  Serial.print("Max. hum:=");
  Serial.println(HUM_MAX);
}

void loopRefresh() {
  #ifdef RTD_NODE
  prev_temp = temperature;
  #endif

  #ifdef DHT_NODE
  prev_dht_temp = dht_temp;
  prev_dht_humidity = dht_hum;
  #endif

  #ifdef CO2_NODE
  prev_co2 = co2;
  #endif

  faultyFlag = 0;
  isThresholdExceeded = 0;
}

void checkThreshold() {
#ifdef RTD_NODE
  float diff_rtd = ABS((temperature - prev_temp));
  if ( diff_rtd >= TEMP_TH ) {
    isThresholdExceeded = 1;
  }
#endif

#ifdef DHT_NODE
  float diff_dht_t = ABS((dht_temp - prev_dht_temp));
  float diff_dht_h = ABS((dht_hum - prev_dht_humidity));
  if (diff_dht_t >= DHT_TEMP_TH || diff_dht_h >= DHT_HUM_TH ) {
    isThresholdExceeded = 1;
  }
#endif

#ifdef CO2_NODE
  float diff_co2 = ABS((co2 - prev_co2));
  if ( diff_co2 >= CO2_TH ) {
    isThresholdExceeded = 1;
  }
#endif
}

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
  co2Sensor.begin(UART2_RX, UART2_TX);
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
  if(!MDNS.begin("esp")) {
    Serial.println("Error starting mDNS");
    return;
  }

  delay(20000);
#ifdef RTD_NODE
  rtd = thermo.readRTD();
#endif
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RTD_PIN, OUTPUT);

  pinMode(BATTERY_CS, OUTPUT);
  pinMode(BATTERY_IN, INPUT);
  digitalWrite(BUZZER_PIN, LOW);

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
}

void loop() {
  server.handleClient();
  FetchAllSetpoints();
  digitalWrite(RTD_PIN, HIGH);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  checkWifi(0);
  Serial.println();

  ReadAllPeripherals();

  displayUpdate(
      // Temperature and Humidity
#if defined(RTD_NODE)
      temperature,
      0,
#elif defined(DHT_NODE)
      dht_temp,
      dht_hum,
#elif defined(CO2_NODE)
      0,
      0,
#endif
      // CO2
#if defined(CO2_NODE)
      co2,
#else
      0,
#endif
      battery
      );

  if(faultyFlag > 0) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(20 * 1000);
    digitalWrite(BUZZER_PIN, LOW);
  }

  n = getTime();
  uint32_t ts = n.unixtime();

  checkThreshold();

  loopRefresh();
  if (isThresholdExceeded) {
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
        ,
        battery
        );
    delay(TS * 1000);
  }
  else {
    uint64_t diff = millis() - lastSend;
    uint64_t STS_millis = STS * 1000;

    if(ABS(diff) >= STS) {
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
          ,
          battery
          );
    }
  }
}
