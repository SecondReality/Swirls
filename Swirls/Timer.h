#ifndef Second_Reality_Engine_Timer
#define Second_Reality_Engine_Timer

#include <windows.h>
#include <list>
#include <limits>

namespace smr   {
//------------------------------------------------------------------------------
// Class Timer
//------------------------------------------------------------------------------
// Desc: A standard timer class. Notice there is no need for a stop function.
//------------------------------------------------------------------------------
// Function: void start();
// Starts the timer. If it is already going it will reset the time.
//------------------------------------------------------------------------------
// Function: void time() const;
// Retrives how long the timer has been running since start() was called.
//------------------------------------------------------------------------------

class Timer
{
public:
	Timer();
	virtual ~Timer();

	void reset();
	float time() const;
	float getSystemTime() const;

private:
	LARGE_INTEGER timed;
	mutable LARGE_INTEGER clockFrequency;
	mutable LARGE_INTEGER temp;
};
//------------------------------------------------------------------------------
// Class loop_var
//------------------------------------------------------------------------------
// Desc: With timers we often want a variable that can loop, but still stay to
// a constrained range.
// Templatised for speed.
template<class T, T Min, T Max>class loop_var
{
	T var;
public:
	loop_var(){}
	loop_var(T val){while(val>Max){val-=Max;} var=val;}
	T operator+=(T t){ return (var+t>Max) ? (var=Min+(Max-t)) : (var+=t);}
	T v(){return var;}
};
//------------------------------------------------------------------------------
// Class WorldTimer
//------------------------------------------------------------------------------
// Desc: A slightly more advanced timer.
// Provides fps and other statistics, and methods
// of altering the time flow rate (think bullet-time).
// The timer cannot be reset.
//------------------------------------------------------------------------------
// Functions linear and delta return the amount of time since the timer
// was created and the time elapsed beween the last two frame calls, resectivly.
// The real versions of these functions return the pure, non-time flow rate 
// modified time, this may be essential for some objects to have.
//------------------------------------------------------------------------------
// Suggested Changes:
// 31st Jan 05 : FPS is a bad indication of performance because it is not
// linear. Use average frame time instead.
//------------------------------------------------------------------------------

class WorldTimer : private Timer
{
public:
  WorldTimer();
  ~WorldTimer();

  void frame(const float &TimeModifier=1.0f);

  float linear() const;
  float delta() const;

  float linear_r() const;	// Linear real time
  float delta_r() const;	// Delta real time

  float fps() const;
  float frame_time() const;
    
private:
  // These variables are affected by the timeModifier variable:
  float linearTime; // The time since the timer was created
  float deltaTime;	// The change in time since the last frame
  
  // These variables are not affected by the timeModifier variables:
  float realLinearTime;
  float realDeltaTime;
  float timeModifier;
  float framespersec;
};

//------------------------------------------------------------------------------
// Class ActionList
//------------------------------------------------------------------------------
// Desc: Can be used to link objects to times.
// When the time is reached the object can be 'applied' using its virtual function.
//------------------------------------------------------------------------------
// Warning:
// If the event currently being visited is deleted, then setNextEvent must be called!
// Is there a point in the post alert option?  - 29th Jan 05 don't understand this comment
// 29th Jan 05: Consider adding a percentageTime() option.
// it returns the time remaining till the next event as a float in the range 0.0-1.0.
// It should also be 'loop corrected' (when you reach the last event in the actionlist).
//------------------------------------------------------------------------------

/*
template <class T> class ActionList 
{
public:
	typedef T Type;
	enum TimeMode					{LinearTime, DeltaTime};
	enum TimeAlertOptions	{PreAlert, PostAlert};
	enum LoopOptions			{NoLoop, Loop};
	enum OvercastOptions	{All, Last};
	
	ActionList();
	~ActionList();

	void set_options(TimeMode, TimeAlertOptions, LoopOptions, OvercastOptions);

	typedef std::pair<T, float> time_pair;
	typedef std::list<time_pair> tp_list; // Time Pair List

	void insert_event_dt(T event, float time);
	void insert_event_lt(T event, float time);
	void frame_al(float interval);

	tp_list& getEventTimes();
	float getTime();
	typename tp_list::iterator getLastEvent();
	typename tp_list::iterator getNextEvent();
	void setNextEvent(tp_list::iterator);

protected:
	virtual void onEvent(T& event, float timeDifference)=0; // Note: timeDifference can be a negative value
	//virtual void onNoEvent(float timeDifference); // We call this when nothign happens.

protected:
	tp_list events;
	tp_list::iterator next_event;
	bool start_over;
	float time;
	float real_time;

	// Options:
	TimeMode					timeMode;
	TimeAlertOptions	timeAlertOptions;
	LoopOptions				loopOptions;
	OvercastOptions		overcastOptions;
};

//------------------------------------------------------------------------------
// ActionList Implementation
//------------------------------------------------------------------------------
template<class T>
ActionList<T>::ActionList():
	start_over(true), time(0.0f), real_time(0.0f)
{

}
//------------------------------------------------------------------------------
template<class T>
ActionList<T>::~ActionList()
{
}
//------------------------------------------------------------------------------
template<class T>
void ActionList<T>::set_options
	(TimeMode tm, TimeAlertOptions tao, LoopOptions lo, OvercastOptions oo)
{
	timeMode=tm;
	timeAlertOptions=tao;
	loopOptions=lo;
	overcastOptions=oo;
}
//------------------------------------------------------------------------------
template<class T>
void ActionList<T>::insert_event_dt(T event, float time)
{
	switch (timeMode)
	{
	case DeltaTime:
		{
		 time_pair tp;
		 tp.first=event;
		 tp.second=time;
		 events.push_back(tp);
		 break;
		}
	case LinearTime:
		{
		 // TODO:
		 break;
		}
	}
}
//------------------------------------------------------------------------------
template<class T>
void ActionList<typename T>::insert_event_lt(T event, float time)
{
	switch (timeMode)
	{
	case DeltaTime:
		{
			// Convert the time value to a delta time:
			for (tp_list::const_iterator i=events.begin(); i!=events.end(); ++i)
				time-=i->second;
			time_pair tp;
			tp.first=event;
			tp.second=time;
			tp_list.push_back(tp);
			break;
		}
	case LinearTime:
		{
			// TODO:
			break;
		}
	}
}
//------------------------------------------------------------------------------
	
template<class T>
void ActionList<T>::frame_al(float interval)
{
	// TODO: Find out how I can tell if an iterator does not point anywhere.
	if (start_over)
	{
		next_event=events.begin();
		start_over=false;
	}

	real_time+=interval;
	time+=interval;

	switch (timeMode)
	{
	case DeltaTime:
		{
			if(next_event==events.end())
			{
				next_event=events.begin();
				//real_time=0.0f;
				//time=0.0f;
			}

			for (;next_event!=events.end(); next_event++)
			{
				if (time > next_event->second) // I'm not sure about this.
				{
					onEvent(next_event->first, next_event->second);
					time-=next_event->second;
				}
				else
				{
					break;
				}
			}
			break;
		}
	case LinearTime:
		{
			break;
		}
	}
}
//------------------------------------------------------------------------------
template<class T>
ActionList<typename T>::tp_list& ActionList<typename T>::getEventTimes()
{
	return events;
}
//------------------------------------------------------------------------------
template<class T>
float ActionList<T>::getTime()
{
	return time;
}
//------------------------------------------------------------------------------
template<class T>
ActionList<typename T>::tp_list::iterator ActionList<typename T>::getLastEvent()
{
	//return (--next_event);
	return (--next_event);
	//return next_event==events.end() ? events.begin() : next_event;
}
//------------------------------------------------------------------------------
template<class T>
ActionList<T>::tp_list::iterator ActionList<T>::getNextEvent()
{
	return next_event;
	//return next_event==events.end() ? events.begin() : next_event;
}
//------------------------------------------------------------------------------
// This is a class that demonstrates ActionList.
class ExampleActionList: public ActionList<std::string>
{
public:
	ExampleActionList()
	{
		set_options(DeltaTime, PostAlert, NoLoop, All);
		insert_event_dt("Hello There.", 1.0f);
		insert_event_dt("You.", 1.0f);
		insert_event_dt("Will.", 1.0f);
		insert_event_dt("See.", 1.0f);
		std::string dot; for (int i=0; i<10; i++){dot.append(".");insert_event_dt(dot, (float)i/5);}
		insert_event_dt("The possibilities for this class!", 5.0f);
	}
	void tick(){frame_al(timer.time());timer.reset();}
protected:
	void onEvent(std::string &event, float timeDifference)
	{
		//OutputDebugString(event.c_str());
	}
private:
	Timer timer;
};
//------------------------------------------------------------------------------
// Don't derive from this class, it simply provides a visual framework
// as to how other interpolation classes should work.
//------------------------------------------------------------------------------
template<class T>
class BinaryInterpolate
{
  // Where distance is a range 0-1
  static T interpolate(const T& one, const T& two, float distance);
};
//------------------------------------------------------------------------------
// Simple way of smoothly interpolating between two values
template<class T>
class CubicInterpolate
{
  static T interpolate(const T& one, const T& two, float distance)
  {
    //return (two-one)*distance+one;
	float ft = distance * 3.1415927f;
	float f = (1.0f - cosf(ft)) * 0.5f;
	return one*(1.0f-f) + two*f;
  }
};
//------------------------------------------------------------------------------
// Very often we want to smoothly tween between two or more values
// This class provides an abstract mechanism for doing this:
// Requirements: Input class must support Interpolation functions:
//               Subtract, multiply, increment.
//------------------------------------------------------------------------------
template<class T>
class InterpolateList : public ActionList< T > , public T
{
public:
  
  InterpolateList()
  {
    c=this;
    set_options(DeltaTime, PostAlert, Loop, All);
    //start_over=true;
  }
  // Advance the timer (not set it to a certain time)
  void tick(float time)
  {
    frame_al(time);
    T & tween=*this;
    float f=(*getNextEvent()).second;
    float percentage_time(getTime()/f); // the time completed
    if(f<getTime())
    {
      // if f is smaller than getTime() we know we're looping around -
      // Auto-correction of this coiuld be put in the EventList class
      // We need to know the maximum time:
      //(*getLastEvent()).second
      ::OutputDebugString("TIMELOOP!\n");
      f=getTime();
    }
    tween=smr::CubicInterpolate<T>::interpolate(from, to, percentage_time);
  }
  void onEvent(T &event, float timeDifference)
	{
    from=to; to=event;
    // Nono....next line might be to
    (*c)=from;
		//OutputDebugString(event.c_str());
	}
private:
  T from; T to;
  T* c; // Easier for notation
};
*/
//------------------------------------------------------------------------------
} // namespace smr

#endif // Second_Reality_Engine_Timer

