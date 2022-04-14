#ifndef _MQ_H
#define _MQ_H

#include "global.h"

class GasSensor {
  byte selfPin;
  float currentValue;
  float R0_factor;

  public:
  GasSensor(byte pin, float r0Factor) {
    this->selfPin = pin;
    this->R0_factor = r0Factor;
    pinMode(this->selfPin, INPUT);
  }

  float getRecentValue() {
    return this->currentValue;
  }

  float readValue(float maxVolt) {
    int value = analogRead(this->selfPin);
    float volts = (((float)value)/1024)*maxVolt;
    float RS_gas = (maxVolt - volts)/volts;
    float R0 = RS_gas / this->R0_factor;
    this->currentValue = R0;
    return R0;
  }

};

#endif
