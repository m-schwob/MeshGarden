#ifndef _MG_ADS1X15_H
#define _MG_ADS1X15_H

// #include <Adafruit_Sensor.h>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>
#include "device.h"

class ADS1X15
{
private:
    Adafruit_ADS1X15 ads1x15;

public:
    ADS1X15();
    void begin();
    float analog_read(uint16_t pin);
};

// REGISTER_DEVICE(ADS1X15, "ADS1X15");

#endif /* _MG_ADS1X15_H */
