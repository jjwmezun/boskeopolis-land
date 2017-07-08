#ifndef TIMER_SIMPLE_H
#define TIMER_SIMPLE_H

#include "timer.h"

class TimerSimple : public Timer
{
	public:
		TimerSimple
		(
			int limit = NORMAL_FRAMES,
			bool starts_on = true    ,
			int start_count = 0      ,
			Direction::Vertical direction = Direction::Vertical::UP
		);
		~TimerSimple();

		void start();

		void event( bool hit );
		bool done() const;
		bool hit() const;
		void stop();

	protected:
		bool done_;
};

#endif // TIMER_SIMPLE_H