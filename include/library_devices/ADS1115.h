#ifndef _MG_ADS1115_H
#define _MG_ADS1115_H

#include <Adafruit_ADS1X15.h>
#include "device.h"


class ADS1115 : public Device
{
private:
    Adafruit_ADS1115 ads;

public:
    ADS1115(DEVICE_CONSTRUCTOR_ARGUMENTS)
        : Device(device_id, hardware_info, pinout, envelop)
    {
        ads.setGain(GAIN_ONE); //set gain to
        if (!ads.begin())
        {
            Serial.println("Failed to initialize ADS.");
        }
        else
            Serial.println("ADS initialized");                                           
    }
};

// REGISTER_DEVICE(ADS1115, "Battery Level Sensor");

#endif /* _MG_ADS1115_H */