#ifndef __TIMER_H__
#define __TIMER_H__

#include <SDL.h>

#define __SECOND 1000
#define __MINUTE (60*__SECOND)

class Timer {
	private:
		Uint32 time;
	public:
		Timer();
		void measure();
		void reset();
		Uint32 getTime();
		Uint32 getElapsed();
};

#endif