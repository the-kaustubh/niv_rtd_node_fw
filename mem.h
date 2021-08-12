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

#define TEMP_MIN_ADDR (300)
#define TEMP_MAX_ADDR (302)

#define CO2_MIN_ADDR (304)
#define CO2_MAX_ADDR (306)

#define HUM_MIN_ADDR (308)
#define HUM_MAX_ADDR (310)

void readEEPROM() {
  SSID = EEPROM.readString(ADDR_SSID);
  PASS = EEPROM.readString(ADDR_PASS);
  HOST = EEPROM.readString(ADDR_HOST);
  UID = EEPROM.readString(ADDR_UID);
  USER = EEPROM.readString(ADDR_USER);
  TS = EEPROM.readInt(ADDR_SAMPLING_RATE);

  TEMP_MIN = EEPROM.readInt(TEMP_MIN_ADDR);
  TEMP_MAX = EEPROM.readInt(TEMP_MAX_ADDR);

  HUM_MIN = EEPROM.readInt(HUM_MIN_ADDR);
  HUM_MAX = EEPROM.readInt(HUM_MAX_ADDR);

  CO2_MIN = EEPROM.readInt(CO2_MIN_ADDR);
  CO2_MAX = EEPROM.readInt(CO2_MAX_ADDR);

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

void updateSetPoint(uint16_t addr, uint16_t min, uint16_t max) {
  EEPROM.writeInt(addr, min);
  EEPROM.writeInt(addr + 2, max);

  EEPROM.commit();
}

#endif
