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
  lcd.print(WiFi.localIP());
  lcd.print(" ");
  lcd.print(HOST);
  lcd.setCursor(0, 1);
  lcd.print("UID: ");
  lcd.print(UID);
  lcd.print(" ");
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.print(" deg");

  lcd.display();
}

#endif
