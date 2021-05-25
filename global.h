#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <Adafruit_MAX31865.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>

#define DEF_UID "000000"
#define DEF_USER "kaustubh"

#define DEF_HOST "192.168.77.127:3000"
#define DEF_SSID ("Kaustubh_Pi")
#define DEF_PASS ("kaustubhPi")

#define PROT "http://"
#define ENDPOINT "/write/reading"

WebServer server(80);
String SSID, PASS;
String HOST, UID, USER;
uint16_t TS;

#endif
