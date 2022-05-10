#ifndef _SOILMOISTURESENSORGROOVEV1_0_H_
#define _SOILMOISTURESENSORGROOVEV1_0_H_

#include <Arduino.h>

struct Measurement{
    bool last = false;
    String type;
    float value;
};

#endif /* _SOILMOISTURESENSORGROOVEV1_0_H_ */