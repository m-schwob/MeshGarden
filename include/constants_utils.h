#ifndef _CONSTANTS_UTILS_H_
#define _CONSTANTS_UTILS_H_

#include "library_devices/ADS1X15.h"

extern ADS1X15 ads;
float analog_read(uint16_t pin);
// float analogRead(uint16_t pin);

struct Time
{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
};
#define EEPROM_SIZE (sizeof(Time) + sizeof(int))

float percentage(float value, float max, float min);

#endif /* _CONSTANTS_UTILS_H_ */