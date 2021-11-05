#ifndef _TIMER_H_
#define _TIMER_H_

#include "SDL/include/SDL.h"

typedef unsigned int uint;

class Timer
{
private:

	uint mStartTicks;
	uint mElapsedTicks;
	float mDeltaTime;
	float mTimeScale;
	float lastTime = 0;

public:

	Timer()
	{
		Reset();
		mTimeScale = 1.0f;
	}

	~Timer()
	{
	}

	void Reset()
	{
		mStartTicks = SDL_GetTicks();
		mElapsedTicks = 0;
		mDeltaTime = 0.0f;
	}

	float getDeltaTime()
	{
		return mDeltaTime;
	}

	void setTimeScale(float t)
	{
		mTimeScale = t;
	}

	float getTimeScale()
	{
		return mTimeScale;
	}

	void setDeltaTime(float t)
	{
		mDeltaTime = t;
	}

	float getExecuteTime(bool second = false)
	{
		if (second)
		{
			return  (SDL_GetTicks() - (SDL_GetTicks() % 1000)) * 0.001f;
		}

		return SDL_GetTicks();
	}

	float Update()
	{
		float frame2frame = SDL_GetTicks() - lastTime;
		lastTime = SDL_GetTicks();

		mElapsedTicks = SDL_GetTicks() - mStartTicks;
		mDeltaTime = mElapsedTicks * 0.001f;

		return (frame2frame * 0.001f);
	}

};

#endif

