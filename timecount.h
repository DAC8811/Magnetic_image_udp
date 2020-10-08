#ifndef TIMECOUNT_H
#define TIMECOUNT_H

#include <windows.h>

class TimeCount
{
public:
    LARGE_INTEGER litmp;
    LONGLONG Qpart1,Qpart2,Useingtime;
    double dfMinus,dfFreq,dfTime;
    TimeCount();

    void count_start();
    double count_end();
};

#endif // TIMECOUNT_H
