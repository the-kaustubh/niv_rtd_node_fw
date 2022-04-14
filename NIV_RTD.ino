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

#define ABS(x) ((x<0) ? (-(x)) : (x))

#ifdef RTD_NODE
#define RTD_PIN 25
#include "temperature.h"
#define RREF 430.0
#define RNOMINAL 100.0
uint32_t rtd;
float temperature;
float prev_temp = 0.0;
#define TEMP_TH (0.5)
#endif

#ifdef DHT_NODE
#include "sht.h"
float dht_temp, dht_hum;
float prev_dht_temp = 0.0;
float prev_dht_humidity = 0.0;
#define DHT_TEMP_TH (0.5)
#define DHT_HUM_TH (0.5)
#endif

#ifdef CO2_NODE
#include "mh_z16_co2.h"
float co2;
float prev_co2 = 0.0;
#define CO2_TH (0.5)
#endif

#ifdef MQ_NODE

#include "mq.h"
float prev_CO = 0.0;
float prev_NO = 0.0;
float prev_H2S = 0.0;

float CO = 0.0;
float NO = 0.0;
float H2S = 0.0;

  #define CO_TH (0.5)
  #define NO_TH (0.5)
  #define H2S_TH (0.5)
#endif

uint8_t isThresholdExceeded = 0;
int battery = 0;
DateTime n;

int faultyFlag = 0;
uint8_t buzzerFlag = 0;
uint8_t buzzerDone = 0;

uint64_t lastSend = 0;

void FetchAllSetpoints() {
  setvalues=fetchSetpoint();
#ifdef RTD_NODE
  TEMP_MIN=setvalues[0];
  TEMP_MAX=setvalues[1];
#endif
#ifdef DHT_NODE
  TEMP_MIN=setvalues[0];
  TEMP_MAX=setvalues[1];
  HUM_MIN=setvalues[2];
  HUM_MAX=setvalues[3];
#endif
#ifdef CO2_NODE
  CO2_MIN=setvalues[4];
  CO2_MAX=setvalues[5];
#endif

#ifdef MQ_NODE
  CO_MIN=setvalues[0];
  CO_MAX=setvalues[1];
  NO_MIN=setvalues[2];
  NO_MAX=setvalues[3];
  H2S_MIN=setvalues[4];
  H2S_MAX=setvalues[5];
#endif
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

#ifdef MQ_NODE
  CO = mq_co.readValue(3.3);
  NO = mq_co.readValue(3.3);
  H2S = mq_co.readValue(3.3);

  if((CO > CO_MAX) || (CO < CO_MIN)) {
    faultyFlag += 1;
  }
  if((NO > NO_MAX) || (NO < NO_MIN)) {
    faultyFlag += 1;
  }
  if((H2S > H2S_MAX) || (H2S < H2S_MIN)) {
    faultyFlag += 1;
  }
#endif

  if (!faultyFlag && buzzerDone) {
    buzzerDone = 0;
  }
  Serial.println("battery:=");
  battery = getBattery();
  Serial.println(battery);
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

  #ifdef MQ_NODE
  prev_NO = NO;
  prev_CO = NO;
  prev_H2S = H2S;
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

#ifdef MQ_NODE
  float diff_CO = ABS((CO - prev_CO));
  if ( diff_CO >= CO_TH ) {
    isThresholdExceeded = 1;
  }
  float diff_NO = ABS((NO - prev_NO));
  if ( diff_NO >= NO_TH ) {
    isThresholdExceeded = 1;
  }
  float diff_H2S = ABS((H2S - prev_H2S));
  if ( diff_H2S >= H2S_TH ) {
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
  String HOSTNAME = "ATECHNO" + UID;
  if(!MDNS.begin(HOSTNAME.c_str())) {
    Serial.println("Error starting mDNS");
    return;
  }

  delay(20000);
#ifdef RTD_NODE
  rtd = thermo.readRTD();
  pinMode(RTD_PIN, OUTPUT);
#endif
  pinMode(BUZZER_PIN, OUTPUT);

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
#ifdef RTD_NODE
  digitalWrite(RTD_PIN, HIGH);
#endif
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
#elif defined(MQ_NODE)
      CO,
      NO,
#endif
      // CO2
#if defined(CO2_NODE)
      co2,
#elif defined(MQ_NODE)
      H2S,
#else
      0,
#endif
      battery
      );

  if(faultyFlag > 0 && !buzzerDone) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(20 * 1000);
    digitalWrite(BUZZER_PIN, LOW);
    buzzerDone = 1;
  }

  n = getTime();
  uint32_t ts = n.unixtime();

  checkThreshold();

  loopRefresh();
  if (isThresholdExceeded) {
    storeData(ts,
#ifdef DHT_NODE
        dht_temp
#elif defined(RTD_NODE)
        temperature
#else
        CO
#endif
        ,
#ifdef CO2_NODE
        co2
#elif defined(MQ_NODE)
        H2S
#else
        0.0
#endif
        ,
#ifdef DHT_NODE
        dht_hum
#elif defined(MQ_NODE)
        NO
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
#elif defined(MQ_NODE)
          CO
#else
          temperature
#endif
          ,
#ifdef CO2_NODE
          co2
#elif defined(MQ_NODE)
          H2S
#else
          0.0
#endif
          ,
#ifdef DHT_NODE
          dht_hum
#elif defined(MQ_NODE)
          NO
#else
          0.0
#endif
          ,
          battery
          );
    }
  }
}
