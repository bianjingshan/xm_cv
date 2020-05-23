#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <math.h> 
#include <stdio.h>
#include "xm_math.h"

CXMRand::CXMRand(void)
{
	srand(unsigned(time(0)));
}

uint32_t CXMRand::rand_(uint32_t start, uint32_t end)
{
	int rand_data;
	if((end-start)>RAND_MAX)
	{
        rand_data=(uint32_t)((uint32_t)(rand()*((uint32_t)RAND_MAX+1))+rand());
	}
	else
	{
		rand_data=rand();
	}
	return start+rand_data%(end-start+1);
}

double getDispersion(double *p_input_data, uint32_t input_data_num)
{	
	if(p_input_data==NULL)
	{
		return 0;
	}
	if(input_data_num<=0)
	{
		return 0;
	}
	double temp_data=0, avg_data, dispersion_data=0;
	for(int iloop=0;iloop<input_data_num;iloop++)
	{
		temp_data+=p_input_data[iloop];
	}
	avg_data=temp_data/input_data_num;

	double temp_d;
    for(uint32_t iloop=0;iloop<input_data_num;iloop++)
	{
		temp_d = p_input_data[iloop] - avg_data;
		if (temp_d > 0)
		{
			dispersion_data += temp_d;
		}
		else
		{
			dispersion_data -= temp_d;
		}
	}
	return (dispersion_data/input_data_num/avg_data);
}

double getAverage(double *p_input_data, uint32_t input_data_num)
{	
	if(p_input_data==NULL)
	{
		return 0;
	}
	if(input_data_num<=0)
	{
		return 0;
	}
	double temp_data=0, avg_data;
    for(uint32_t iloop=0;iloop<input_data_num;iloop++)
	{
		temp_data+=p_input_data[iloop];
	}
	avg_data=temp_data/input_data_num;
	return avg_data;
}

