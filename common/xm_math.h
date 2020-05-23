#pragma once

#include <stdint.h>

double getDispersion(double *p_input_data, uint32_t input_data_num);
double getAverage(double *p_input_data, uint32_t input_data_num);

// rand: [start, end]
class CXMRand
{
public:
    CXMRand(void);
    uint32_t rand_(uint32_t start, uint32_t end);
};

#define MIN(x,y) ((x>y)? y:x)
#define MAX(x,y) ((x>y)? x:y)


