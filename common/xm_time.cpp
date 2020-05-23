#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32

#else
#include <sys/time.h>
#endif
#include "xm_time.h"

XMTime::XMTime(void)
{
	start();
}

void XMTime::start(void)
{
    m_start_time = currentTimeUsec();
}

double XMTime::timerSec()
{
    return (currentTimeUsec() - m_start_time) / 1000000.0;
}

double XMTime::timerMsec()
{
    return (currentTimeUsec() - m_start_time) / 1000.0;
}

double XMTime::timer()
{
    return timerSec();
}

uint64_t XMTime::timerUsec()
{
    return currentTimeUsec() - m_start_time;
}

uint64_t XMTime::currentTimeUsec(void)
{
#ifdef _WIN32
    return ((uint64_t)clock() * (uint64_t)1000000 / (uint64_t)CLOCKS_PER_SEC);
#else
	struct timeval t_start;
	gettimeofday(&t_start, NULL);
    return (uint64_t)t_start.tv_sec * (uint64_t)1000000 + (uint64_t)t_start.tv_usec;
#endif
}

uint64_t XMTime::currentTimeSec()
{
#ifdef _WIN32
    return ((uint64_t)clock() / (uint64_t)CLOCKS_PER_SEC);
#else
    struct timeval t_start;
    gettimeofday(&t_start, NULL);
    return (uint64_t)t_start.tv_sec;
#endif
}

const char *XMTime::timeString(uint64_t time_sec, int time_format)
{
    if (!timeString(m_string_buffer, time_sec, time_format))
    {
        m_string_buffer[0] = 0;
    }
    return m_string_buffer;
}

bool XMTime::timeString(char *p_buffer, uint64_t time_sec, int time_format)
{
    time_t temp_time = time_sec;
    struct tm *tt = localtime(&temp_time);
    if (tt == NULL)
    {
        return false;
    }
    if(time_format==0)
    {
        sprintf(p_buffer, "%04u-%02u-%02u %02u:%02u:%02u",
                tt->tm_year+1900,
                tt->tm_mon+1,
                tt->tm_mday,
                tt->tm_hour,
                tt->tm_min,
                tt->tm_sec);
    }
    else if(time_format==1)
    {
        sprintf(p_buffer, "%04u%02u%02u%02u%02u%02u",
                tt->tm_year+1900,
                tt->tm_mon+1,
                tt->tm_mday,
                tt->tm_hour,
                tt->tm_min,
                tt->tm_sec);
    }
    else
    {
        return false;
    }
    return true;
}

bool XMTime::dateTime(uint64_t time_sec, XMDTime_t *date_time)
{
    time_t temp_time = time_sec;
    struct tm *tt = localtime(&temp_time);
    if (tt == NULL)
    {
        return false;
    }
    date_time->year=tt->tm_year+1900;
    date_time->mon=tt->tm_mon+1;
    date_time->mday=tt->tm_mday;
    date_time->hour=tt->tm_hour;
    date_time->min=tt->tm_min;
    date_time->sec=tt->tm_sec;
    return true;
}



