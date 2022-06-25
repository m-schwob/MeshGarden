#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }
  
  // Deep sleep mode for 30 seconds, the ESP8266 wakes up by itself when GPIO 16 (D0 in NodeMCU board) is connected to the RESET pin
  Serial.println("I'm awake");
  int d = millis();
  Serial.println(millis());
  Serial.println();
  delay(1000);
  Serial.println("I'm awake, but I'm going into deep sleep mode for 5 seconds");
  ESP.deepSleep(5e6); 


  // Deep sleep mode until RESET pin is connected to a LOW signal (for example pushbutton or magnetic reed switch)
  //Serial.println("I'm awake, but I'm going into deep sleep mode until RESET pin is connected to a LOW signal");
  //ESP.deepSleep(0); 
}

void loop() {
}