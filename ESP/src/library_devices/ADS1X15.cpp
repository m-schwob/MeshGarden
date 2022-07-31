#include "library_devices/ADS1X15.h"

ADS1X15::ADS1X15() {}

void ADS1X15::begin()
{
    ads1x15.setGain(GAIN_ONE); // set gain to +/- 4.096 range
    if (!ads1x15.begin(0x4A))  // use address for (ADDR <-> SDA)
    {
        Serial.println("Failed to initialize ADS.");
    }
    else
        Serial.println("ADS initialized");
}

float ADS1X15::analog_read(uint16_t pin)
{
    int16_t adc0 = ads1x15.readADC_SingleEnded(pin);
    float volts0 = ads1x15.computeVolts(adc0);
    Serial.print("extender measure: ");
    Serial.println(String(volts0) + "V");
    return volts0;
}