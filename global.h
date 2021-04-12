#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <Adafruit_MAX31865.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>

#define UID "100000"
#define USER "kaustubh"

#define PROT "http://"
#define HOST "192.168.155.127:3000"
#define ENDPOINT "/write/reading"

WebServer server(80);

#endif
