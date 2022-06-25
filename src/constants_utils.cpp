#ifndef _CONSTANTS_UTILS_H_
#define _CONSTANTS_UTILS_H_


float percentage(float value, float max, float min = 0)
    {
        if (min > value || value > max)
            return -1;
        return (value - min) / (max - min);
    }

#endif /* _CONSTANTS_UTILS_H_ */