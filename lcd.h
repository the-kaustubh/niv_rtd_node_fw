#ifndef _LCD_H
#define _LCD_H

#include "global.h"

void lcdSetup() {
  lcd.init();
  lcd.backlight();
}

void displayUpdate(float temperature) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Device UID: ");
  lcd.print(UID);
  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.print(" Celsius");
  lcd.print(" ");
  lcd.setCursor(0, 2);
  if(WiFi.status() == WL_CONNECTED) {
    lcd.print("N/W OK");
  } else {
    lcd.print("Offline");
  }
  lcd.print(" ");
  lcd.setCursor(0, 3);
  lcd.print(WiFi.localIP());

  lcd.display();
}

#endif
