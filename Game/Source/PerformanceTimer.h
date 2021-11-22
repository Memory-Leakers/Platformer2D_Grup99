#ifndef __PERFORMANCETIMER_H__
#define __PERFORMANCETIMER_H__

#include "Defs.h"

class PerformanceTimer
{
public:

	PerformanceTimer();

	void Start();

	double ReadMs() const;

	uint64 ReadTicks() const;

private:
	uint64 startTimer;
	static uint64 frequency;

};

#endif //__PERFORMANCETIMER_H__
