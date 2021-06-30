#ifndef NDIR_CO2_H
#define NDIR_CO2_H

#include "global.h"

#define UART2_RX (16)
#define UART2_TX (17)
NDIR_SoftwareSerial co2Sensor(UART2_RX, UART2_TX);

#endif /* ifndef NDIR_CO2_H */
