#ifndef _PINSMAPS_H_
#define _PINSMAPS_H_

#include <Arduino.h>
#include <ArduinoJson.h>

// pins can digital and analog. in that case, the capacity size will be grater then needed and will be shrink later
#define PINS_MAP_SIZE JSON_OBJECT_SIZE(NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS + 1) //+1 for LED_BUILTIN

extern DynamicJsonDocument pins_map;
void _map_pins();
void map_pins();
uint8_t pin(String pin);

#endif /* _PINSMAPS_ */