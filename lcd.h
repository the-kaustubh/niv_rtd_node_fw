#ifndef _LCD_H
#define _LCD_H

#include "global.h"

void lcdSetup() {
  lcd.init();
  lcd.backlight();
}

void displayUpdate(float temperature, float humidity, float co2,  float battery) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Device UID: ");
  lcd.print(UID);
  // =======
  lcd.setCursor(0, 1);
  // =======
  if(WiFi.status() == WL_CONNECTED) {
    lcd.print("Signal Strength: HI");
    /* lcd.print(WiFi.localIP()); */
  } else {
    lcd.print("Signal Strength: LO");
  }
  // =======
  lcd.setCursor(0, 2);
  // =======
#if defined(RTD_NODE)
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.print(" C");
  lcd.print(" ");
#elif defined(DHT_NODE)
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.print(" C H: ");
  lcd.print(humidity);
  lcd.print(" %");
  lcd.print(" ");
#endif
#if defined(CO2_NODE)
  lcd.setCursor(0, 3);
  lcd.print("CO2: ");
  lcd.print(co2);
#endif

#if defined(MQ_NODE)
  lcd.print("CO: ");
  lcd.print(temperature);
  lcd.print(" ppm");
  lcd.print(" ");
  lcd.print("NO: ");
  lcd.print(humidity);
  lcd.print(" ppm");
  lcd.print(" ");

  lcd.setCursor(0, 3);
  lcd.print("H2S: ");
  lcd.print(co2);
  lcd.print(" ppm");
  lcd.print(" ");
#endif

  // =======
  lcd.print("BAT: ");
  lcd.print(battery);

  lcd.display();
}

#endif
