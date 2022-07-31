#include <painlessMesh.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_ADS1X15.h>
//DO NOT REMOVE INCLUDES UNLESS UPDATING INI FILE!!!!


#define BAUD_RATE 115200 

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */


//Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */

// ** TODO put give special care to this code when making ADS1X15 driver**//
// // Pin connected to the ALERT/RDY signal for new sample notification.
// constexpr int READY_PIN = 3;

// // This is required on ESP32 to put the ISR in IRAM. Define as
// // empty for other platforms. Be careful - other platforms may have
// // other requirements.
// #ifndef IRAM_ATTR
// #define IRAM_ATTR
// #endif

// volatile bool new_data = false;
// void IRAM_ATTR NewDataReadyISR() {
//   new_data = true;
// }
// ** END TODO **//

void setup(void)
{
  Serial.begin(BAUD_RATE);
  Serial.println("Hello!");

  Serial.println("Getting reading from AIN0 (P) and AIN1 (N)");
  Serial.println("ADC Range: +/- 4.096V  1 bit = 0.125mV");
  //TODO (1) old code make macro for right string
  //Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");

  // TOTO make that defined by macro?
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.setGain(GAIN_ONE);           // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  pinMode(D4,OUTPUT);
  digitalWrite(D4, HIGH); 
  
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    //while (1);
  }

  // TODO part of TODO (1)
  // pinMode(READY_PIN, INPUT);
  // We get a falling edge every time a new sample is ready.
  //attachInterrupt(digitalPinToInterrupt(READY_PIN), NewDataReadyISR, FALLING);

  // example code
  // Start continuous conversions.
  //ads.startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, /*continuous=*/true);
  //ads.startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, /*continuous=*/true);

}

void loop(void)
{
  // TODO part of TODO (1)
  // If we don't have new data, skip this iteration.
  // if (!new_data) {
  //   return;
  // }

  int16_t adc0 = ads.readADC_SingleEnded(0);
  float volts0 = ads.computeVolts(adc0);

  Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0); Serial.println("V");

  // TODO part of TODO (1)
  // new_data = false;

  // In a real application we probably don't want to do a delay here if we are doing interrupt-based sampling, but we have a delay
  // in this example to avoid writing too much data to the serial port.
  delay(1000);
}