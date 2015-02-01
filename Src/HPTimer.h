//Author: Sören Pirk
//Date: 22.01.2013

#ifndef HPTIMER
#define HPTIMER

#include <windows.h>

class HPTimer
{
private:
    LARGE_INTEGER t0;
    LARGE_INTEGER t1;
    LARGE_INTEGER f;

    double et,last_dt;
    bool na;

public:
    HPTimer(); 
    void reset();
    double time();
};

#endif