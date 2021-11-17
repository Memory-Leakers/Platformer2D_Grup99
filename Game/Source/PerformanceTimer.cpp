#include "PerformanceTimer.h"
#include "SDL\include\SDL_timer.h"


uint64 PerformanceTimer::frequency = 0;

PerformanceTimer::PerformanceTimer()
{
	Start();
	frequency = SDL_GetPerformanceFrequency();
}

void PerformanceTimer::Start()
{
	startTimer = SDL_GetPerformanceCounter();
}

double PerformanceTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - startTimer) / double(frequency));
}

uint64 PerformanceTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - startTimer;
}