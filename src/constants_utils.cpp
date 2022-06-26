#include "constants_utils.h"

ADS1X15 ads;

float analog_read(uint16_t pin)
{
    return ads.analog_read(pin);
}
// float analogRead(uint16_t pin)
// {
//     return ads.analog_read(pin);
// }

float percentage(float value, float max, float min = 0)
    {
        if (min > value || value > max)
            return -1;
        return (value - min) / (max - min);
    }

