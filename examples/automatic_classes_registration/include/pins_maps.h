#ifndef _PINSMAPS_H_
#define _PINSMAPS_H_

#include <Arduino.h>
#include <ArduinoJson.h>

//     PINS MAPPING     //
#if defined(ESP8266)

// esp8266 pinout map builder
#define PINS_MAP_SIZE 1 * JSON_OBJECT_SIZE(19) + 19 * JSON_OBJECT_SIZE(1) + 14 * JSON_STRING_SIZE(2) + 2 * JSON_STRING_SIZE(3) + 2 * JSON_STRING_SIZE(4) + 1 * JSON_STRING_SIZE(11)

#elif defined(ESP32)

// esp32 pinout map builder
#define PINS_MAP_SIZE 1 * JSON_OBJECT_SIZE(41) + 39 * JSON_OBJECT_SIZE(1) + 22 * JSON_STRING_SIZE(2) + 12 * JSON_STRING_SIZE(3) + 4 * JSON_STRING_SIZE(4) + 1 * JSON_STRING_SIZE(11)

#endif

extern DynamicJsonDocument pins_map;
void _map_pins();
void map_pins();

#endif /* _PINSMAPS_ */