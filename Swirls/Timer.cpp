// Timer.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Timer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace smr;

//------------------------------------------------------------------------------
// Class Timer
//------------------------------------------------------------------------------
Timer::Timer()
{
  timed.QuadPart=0;
  temp.QuadPart=0;
	if(QueryPerformanceFrequency(&clockFrequency)==0)
	{
		// TODO: Use different timer method(?)
		// Possibly use GetLastError
	}
	QueryPerformanceCounter(&timed);
 }

Timer::~Timer()
{

}

void Timer::reset()
{
	QueryPerformanceCounter(&timed);
}

float Timer::time() const
{
  //QueryPerformanceFrequency(&clockFrequency);
	QueryPerformanceCounter(&temp);
	//return (static_cast<float>(temp.QuadPart-timed.QuadPart)/static_cast<float>(clockFrequency.QuadPart));
  return (float)( (double)(temp.QuadPart-timed.QuadPart)/(double)(clockFrequency.QuadPart));
}

float Timer::getSystemTime() const
{
	return (float)timed.QuadPart/clockFrequency.QuadPart;
}

//------------------------------------------------------------------------------
// Class WorldTimer
//------------------------------------------------------------------------------
WorldTimer::WorldTimer():
  timeModifier(0.0f),
  linearTime(0.0f),
  deltaTime(0.0f),
  realLinearTime(0.0f),
  realDeltaTime(0.0f)
{
}
//------------------------------------------------------------------------------
WorldTimer::~WorldTimer()
{
}
//------------------------------------------------------------------------------
void WorldTimer::frame(const float & rhs_timeModifier)
{
  // As of yet there is no restrictions on the 
  // time modifier value (negative is valid).
  timeModifier=rhs_timeModifier;

	// Check the interval, then start timing again.
  realDeltaTime=time();
	reset();
	
	realLinearTime+=realDeltaTime;

	// Compute the time affected variables:
	deltaTime=realDeltaTime * timeModifier;
	linearTime+=deltaTime;
}
//------------------------------------------------------------------------------
float WorldTimer::linear() const
{
  return linearTime;
}
//------------------------------------------------------------------------------
float WorldTimer::delta() const
{
  return deltaTime;
}
//------------------------------------------------------------------------------
float WorldTimer::linear_r() const
{
	return realLinearTime;
}
//------------------------------------------------------------------------------
float WorldTimer::delta_r() const
{
	return realDeltaTime;
}
//------------------------------------------------------------------------------
float WorldTimer::fps() const
{
	return 1000.0f/realLinearTime;
}
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------