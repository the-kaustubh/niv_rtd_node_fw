#ifndef _BAT_H
#define _BAT_H

#include "global.h"

float getBattery() {
  digitalWrite(BATTERY_CS, HIGH);
  delay(200);
  float analogBat = analogRead(BATTERY_IN);
  delay(200);
  digitalWrite(BATTERY_CS, LOW);
  return map(analogBat, 0, 4095, 0, 100);

}

#endif
