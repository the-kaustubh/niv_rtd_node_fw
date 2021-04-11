#include <Adafruit_MAX31865.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>

#define UID "100000"
#define USER "kaustubh"

#define PROT "http://"
#define HOST "192.168.43.213:3000"
#define ENDPOINT "/write/reading"
