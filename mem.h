#ifndef _EEP_H
#define _EEP_H

#include "global.h"
#include "wifi.h"

#define ADDR_SSID (0)
#define ADDR_PASS (50)
#define ADDR_UID (100)
#define ADDR_HOST (110)
#define ADDR_USER (170)
#define ADDR_SAMPLING_RATE (220)
#define ADDR_EEPROM_CONFIG (280)

void readEEPROM() {
  SSID = EEPROM.readString(ADDR_SSID);
  PASS = EEPROM.readString(ADDR_PASS);
  HOST = EEPROM.readString(ADDR_HOST);
  UID = EEPROM.readString(ADDR_UID);
  USER = EEPROM.readString(ADDR_USER);
  TS = EEPROM.readInt(ADDR_SAMPLING_RATE);
}

void checkEEPROM() {
  if(EEPROM.readByte(ADDR_EEPROM_CONFIG) == 255) {
    EEPROM.writeString(ADDR_SSID, DEF_SSID);
    EEPROM.writeString(ADDR_PASS, DEF_PASS);
    EEPROM.writeString(ADDR_HOST, DEF_HOST);
    EEPROM.writeString(ADDR_UID, DEF_UID);
    EEPROM.writeString(ADDR_USER, DEF_USER);
    EEPROM.writeInt(ADDR_SAMPLING_RATE, 3);

    EEPROM.writeByte(ADDR_EEPROM_CONFIG, 1);
    EEPROM.commit();
  }
  readEEPROM();
}

void updateSSID(const char *i) {
  EEPROM.writeString(ADDR_SSID, i);
  EEPROM.commit();
}
void updatePASS(const char *i) {
  EEPROM.writeString(ADDR_PASS, i);
  EEPROM.commit();
}
void updateHOST(const char *i) {
  EEPROM.writeString(ADDR_HOST, i);
  EEPROM.commit();
}
void updateUID(const char *i) {
  EEPROM.writeString(ADDR_UID, i);
  EEPROM.commit();
}
void updateUSER(const char *i) {
  EEPROM.writeString(ADDR_USER, i);
  EEPROM.commit();
}
void updateTS(const char *i) {
  EEPROM.writeInt(ADDR_SAMPLING_RATE, atoi(i));
  EEPROM.commit();
}

#endif
