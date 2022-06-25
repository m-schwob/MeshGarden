#include "library_devices/ADS1X15.h"

ADS1X15::ADS1X15(DEVICE_CONSTRUCTOR_ARGUMENTS) : Device(device_id, hardware_info, pinout, envelop){
    ads.setGain(GAIN_ONE); // set gain to +/- 4.096 range
    if (!ads.begin(0x4A))  // use address for (ADDR <-> SDA)
    {
        Serial.println("Failed to initialize ADS.");
    }
    else
        Serial.println("ADS initialized");
}

float ADS1X15::analogRead(uint16_t pin)
{
    int16_t adc0 = ads.readADC_SingleEnded(pin);
    float volts0 = ads.computeVolts(adc0);
    Serial.print("extender measure: ");
    Serial.println(String(volts0) + "V");
    return volts0;
}