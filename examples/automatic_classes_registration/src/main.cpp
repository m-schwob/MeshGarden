#include <Arduino.h>
#include "mesh_garden.h"
    
/* 
This documentation will be after use for user instruction and documentation. but for now its here to simplify. 

For custom sensor, user need to provide the following information:
- A function in the .ino file that know how to init the sensor (called one time when the system boot up)
- A function in the .ino file that know how to handle the measuring proccess and return the measured value in the correct units.
    - To support sensor that measure more then one value the return value must be an array. The array will use Measurement type.
- An hardware id that will also add to the app. (that will be used to init the correct sensor in run time).
- A information that will be add via the app (option for json file?):
    - hardware id - short String that uniquely represent the sensor. must matched the hardware id mentioned before. 
    - pinout - the how the sensor connected to the board. for example if pin 'DAT' of the sensor connected to pin 'D1' of the
                 board, both values will linked via the app and in the above functions it will be use using pin(Sting pin_name) 
                 function, for example: pinMode(pin("DAT"), INPUT), readDigital(pin("DAT")).
    - measurement_types - a list of Strings. each represent one measurement type to handle multi measurements sensors.
    - units - a list of Strings. units of each measurement_type.
    - time_interval - the time intervale between preformed measurements

For the .ino file, he idea is to give the user two way of adding his own sensor:
1. Beginners options - implementing two functions 'void init_sensor()' and 'Measurement measure()' and registering the sensor using 
    provided library function 'add_sensor(String hardware_id, InitFunc init_sensor_func, MeasureFunc measure_func)'. 
    (that will make a class in the background)
2. Advanced options - making a custom class inheriting the sensor class and implementing the above functions and registering the sensor 
        using. This option gives user more power to override default behaviors. (consider supporting this for this project)

*/

#define BAUD_RATE 115200

MeshGarden garden;

void init_sensor1(){
    // code to init the sensor
    // example of using Arduino 'pinMode' function
    pinMode(pin("DAT"), INPUT);
}

Measurements measure_sensor1(){
    // code that do the measurement
    // example of using Arduino 'readDigital' function
    digitalRead(pin("DAT"));

    return Measurements(); //temporary for compiling
}

void setup()
{   
    Serial.begin(115200);
    Serial.println();

    garden.add_sensor("example_sensor1", init_sensor1, measure_sensor1);
    garden.begin();
}

void loop()
{
    garden.update();
    delay(5000);// temporary
}