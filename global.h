#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <Adafruit_MAX31865.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Wire.h>
#include <MHZ16_uart.h>
#include <DHT.h>
#include <ESPmDNS.h>

#define DEF_UID "000000"
#define DEF_USER "kaustubh"

#define DEF_HOST "192.168.77.127:3000"
#define DEF_SSID ("Kaustubh_Pi")
#define DEF_PASS ("kaustubhPi")

#define PROT "http://"
#define ENDPOINT "/write/reading"

#define uS_to_S (1000000)

#define DHTTYPE DHT11
#define DHTPIN (4)

#define BATTERY_IN (25)
#define BATTERY_CS (26)

#define BUZZER_PIN (34)

WebServer server(80);
LiquidCrystal_I2C lcd(0x27, 20, 4);
String SSID, PASS;
String HOST, UID, USER;
Adafruit_MAX31865 thermo = Adafruit_MAX31865(27, 14, 12, 13);
DHT dht(DHTPIN, DHTTYPE);
uint16_t TS;

#endif
