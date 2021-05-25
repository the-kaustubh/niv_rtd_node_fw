#ifndef _ROUTES_H
#define _ROUTES_H

#include "global.h"
#include "mem.h"

#define SAVE_PARAM(ARG, FUNC) \
	if(server.arg(ARG) != "") { \
		FUNC(server.arg(ARG).c_str()); \
		Serial.println(server.arg(ARG));	\
	}

#define SEND_PARAM(KEY, VAL) \
	"\"" #KEY "\": " + String(VAL) + ""

#define JSON_SEP ","


const char CONF[] PROGMEM = "<!DOCTYPE html><html><head>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>Configure</title><style>"
"body{font-family:Helvetica,Arial;}"
"hr{color: #000; width:90%;border:none;}"
"div{margin:20px auto;border:1px black solid;width:300px;"
"text-align:center;background-color:#f5f2d0;border-radius:5px;}"
"input,select{width:90%;margin:5px 0;padding:5px;text-align:center;"
"resize:none;border:1px #333 solid;border-radius:3px;}"
".dmin{width:40%;margin:0 5px;}"
".subm{background-color:#3498DB;color:#fff}</style></head>"
"<body><form method='get' action='/save'>"
"<div><h1> Configure </h1>"
"<input name='ssid' placeholder='SSID'>"
"<input name='pass' placeholder='Password'><hr>"
"<input name='uid' placeholder='UID'>"
"<input name='host' placeholder='URL'>"
"<input name='user' placeholder='User'>"
"<input name='ts' placeholder='Sampling time'>"
"<input class='subm' type='submit' value=' OK '> <hr>"
"</div></form</body></html>";

void printParam(const char * p) {
  Serial.println(p);
}


void handleRoot() {
  server.send(200, "text/html", CONF);

}

void handleNotFound() {
  server.send(404, "text/html", "<b>Not Found</b>");
}

void handleSave() {
  SAVE_PARAM("ssid", updateSSID);
  SAVE_PARAM("pass", updatePASS);
  SAVE_PARAM("uid",  updateUID);
  SAVE_PARAM("host",  updateHOST);
  SAVE_PARAM("user",  updateUSER);
  SAVE_PARAM("ts",   updateTS);

  server.send(201, "text/html", "Your config was saved <a hreaf='/'>Go back</a>");
  ESP.restart();

}

#endif
