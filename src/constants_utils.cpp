#include "constants_utils.h"

#if defined(ESP8266)
ADS1X15 ads;

float analog_read(uint16_t pin)
{
    return ads.analog_read(pin);
}
#else // ugly plaster fo esp32
float analog_read(uint16_t pin)
{
    return 0.0;
}
#endif

// float analogRead(uint16_t pin)
// {
//     return ads.analog_read(pin);
// }

float percentage(float value, float max, float min = 0)
{
    if (min > value || value > max)
        return -1;
    return 100 * (value - min) / (max - min);
}
