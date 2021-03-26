#ifndef _RTC_A_H
#define _RTC_A_H

#include <RTClib.h>

RTC_DS3231 rtc;
DateTime now;

void rtcBegin() {
  if(!rtc.begin()) {
    Serial.println("Could Not Find RTC");
  }

  if(rtc.lostPower()) {
    Serial.println("RTC has lost power.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

DateTime getTime() {
  return rtc.now();
}



#endif
