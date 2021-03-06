#pragma once

#include <stdint.h>

typedef struct
{
    int year;
    int mon;
    int mday;
    int hour;
    int min;
    int sec;
}XMDTime_t;

class TickTime
{
public:
    TickTime(void);
    void start(void);
    double timerSec(void);
    double timerMsec(void);
    double timer(void);
    uint64_t timerUsec(void);

    static uint64_t currentTimeUsec(void);
    static uint64_t currentTimeSec(void);
    static uint32_t currentTimeMsecCut(void);
    static void currentDateTime(XMDTime_t *date_time);
    static void currentTimeString(char *buffer, int time_format = 0);

    // time_format:
    // 0 = 2017-03-01 10:53:00
    // 1 = 20170301105300
    // 2 = 2017-03-01 10:53:00:001
    const char * timeString(uint64_t time_sec, uint32_t ms, int time_format=0);
    static bool timeString(char *p_buffer, uint64_t time_sec, uint32_t ms, int time_format=0);
    static bool dateTime(uint64_t time_sec, XMDTime_t *date_time);

private:
    uint64_t m_start_time;
    char m_string_buffer[64];
};


