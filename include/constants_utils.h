#ifndef _CONSTANTS_UTILS_H_
#define _CONSTANTS_UTILS_H_

struct Time
{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
};
#define EEPROM_SIZE (sizeof(Time) + sizeof(int))

float percentage(float value, float max, float min = 0);

#endif /* _CONSTANTS_UTILS_H_ */