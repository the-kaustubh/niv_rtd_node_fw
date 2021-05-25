#ifndef _LOG_H
#define _LOG_H

#include "global.h"
#include <SD.h>
#include <FS.h>

#define CS_PIN (5)

#define FILE_SAVE ("/temperature")

int sdBegin() {
  if(!SD.begin(CS_PIN)) {
    Serial.println("Card Mount Failed");
    return 1;
  }
  Serial.println("SD Card mounted successfully.");
  return 0;
}

int readingsPresent() {
  if (SD.exists(FILE_SAVE)) {
    return 1;
  }
  return 0;
}

int writeReading(uint32_t tstamp, float reading) {
  File ts;
  char tsr[20];
  if(readingsPresent()) {
    ts = SD.open(FILE_SAVE, FILE_APPEND);
  } else {
    ts = SD.open(FILE_SAVE, FILE_WRITE);
  }
  if ( !ts ) {
    Serial.println("Couldn't open File");
    return 0;
  }
  snprintf(tsr, 20, "%ld,%2.2f\r\n", tstamp, reading);
  Serial.println(tsr);

  int bytes_written = ts.print(tsr);
	if(!bytes_written) {
		Serial.println("Timestamp Write Failed");
	}
	Serial.print(bytes_written);
	Serial.println(" bytes were written.");
	Serial.println("Closing file /temperature");
	ts.close();
}

int clearFile(const char * file) {
  SD.remove(file);
  File x = SD.open(file, FILE_WRITE);
  x.close();
}

String readAllTimestamps() {
  String fmt = "";
	File ts = SD.open(FILE_SAVE, FILE_READ);
	if(!ts) {
		Serial.println("File Not Found.");
		return "";
	}
	while(ts.available()) {
    fmt += ts.read();
	}
  ts.close();
  clearFile(FILE_SAVE);
	return fmt;
}
#endif
