#ifndef _MG_ADS1X15_H
#define _MG_ADS1X15_H

#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>
#include "device.h"

class ADS1X15 : public Device
{
private:
    Adafruit_ADS1X15 ads;

public:
    ADS1X15(DEVICE_CONSTRUCTOR_ARGUMENTS);

    float analog_read(uint16_t pin);
};

// REGISTER_DEVICE(ADS1X15, "ADS1X15");

#endif /* _MG_ADS1X15_H */