#include "pins_maps.h"

DynamicJsonDocument pins_map(PINS_MAP_SIZE);

#if defined(ESP8266)
// esp8266 pinout map builder
void _map_pins()
{
    pins_map["LED_BUILTIN"] = LED_BUILTIN;

    pins_map["RX"] = RX;
    pins_map["TX"] = TX;

    pins_map["SDA"] = SDA;
    pins_map["SCL"] = SCL;

    pins_map["SS"] = SS;
    pins_map["MOSI"] = MOSI;
    pins_map["MISO"] = MISO;
    pins_map["SCK"] = SCK;

    pins_map["A0"] = A0;

    pins_map["D0"] = D0;
    pins_map["D1"] = D1;
    pins_map["D2"] = D2;
    pins_map["D3"] = D3;
    pins_map["D4"] = D4;
    pins_map["D5"] = D5;
    pins_map["D6"] = D6;
    pins_map["D7"] = D7;
    pins_map["D8"] = D8;
}

#elif defined(ESP32)
// esp32 pinout map builder
void _map_pins()
{
    pins_map["LED_BUILTIN"] = LED_BUILTIN;

    pins_map["TX"] = TX;
    pins_map["RX"] = RX;

    pins_map["SDA"] = SDA;
    pins_map["SCL"] = SCL;

    pins_map["SS"] = SS;
    pins_map["MOSI"] = MOSI;
    pins_map["MISO"] = MISO;
    pins_map["SCK"] = SCK;

    pins_map["A0"] = A0;
    pins_map["A3"] = A3;
    pins_map["A4"] = A4;
    pins_map["A5"] = A5;
    pins_map["A6"] = A6;
    pins_map["A7"] = A7;
    pins_map["A10"] = A10;
    pins_map["A11"] = A11;
    pins_map["A12"] = A12;
    pins_map["A13 "] = A13;
    pins_map["A14 "] = A14;
    pins_map["A15 "] = A15;
    pins_map["A16 "] = A16;
    pins_map["A17 "] = A17;
    pins_map["A18 "] = A18;
    pins_map["A19 "] = A19;

    pins_map["T0"] = T0;
    pins_map["T1"] = T1;
    pins_map["T2"] = T2;
    pins_map["T3"] = T3;
    pins_map["T4"] = T4;
    pins_map["T5"] = T5;
    pins_map["T6"] = T6;
    pins_map["T7"] = T7;
    pins_map["T8"] = T8;
    pins_map["T9"] = T9;

    pins_map["DAC1"] = DAC1;
    pins_map["DAC2"] = DAC2;
}

#endif

void map_pins()
{
    _map_pins();
    size_t capacity_before = pins_map.capacity();
    pins_map.shrinkToFit();
    Serial.println("pins map created:");
    Serial.println(pins_map.as<String>());
    Serial.print("capacity before shrinking: ");
    Serial.print(String(capacity_before));
    Serial.print(", capacity after shrinking: ");
    Serial.print(String(pins_map.capacity()));
    Serial.print(", actual memory usage: ");
    Serial.println(String(pins_map.memoryUsage()));
}
