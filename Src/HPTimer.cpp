//Author: Sören Pirk
//Date: 22.01.2013

#include "HPTimer.h"

HPTimer::HPTimer()
{
	QueryPerformanceFrequency(&f);
	na=f.QuadPart==0;
	reset();
}

void HPTimer::reset()
{
	QueryPerformanceCounter(&t0);
	last_dt=0.0;
	et=0.0;
}

double HPTimer::time()
{
	double dt;

	if (na) return 0.0;

	QueryPerformanceCounter(&t1);
	dt=double(t1.QuadPart-t0.QuadPart);
	if (dt<0.0)
	{
		et=et+last_dt/f.QuadPart;
		t0=t1;
		last_dt=0.0;
		return et;
	}
	else
	{
		last_dt=dt;
		return et+dt/f.QuadPart;
	}
}