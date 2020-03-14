#pragma once

#include "timer_t.hpp"

template <int limit_T = TIMER_T_DEFAULT_FRAMES, bool starts_on_T = true, int start_count_T = 0, Direction::Vertical direction_T = Direction::Vertical::UP>
class TimerSimpleT final : public TimerT<limit_T, starts_on_T, start_count_T, direction_T>
{
	public:
		constexpr TimerSimpleT()
	    :
	        TimerT<limit_T, starts_on_T, start_count_T, direction_T> (),
			done_ ( false )
	    {};
		~TimerSimpleT() {};

		void start() override
	    {
	        this->on_ = true;
	        this->restartCounter();
	        this->done_ = false;
	    };

		void event( bool hit ) override
	    {
	        if ( hit )
	        {
	            this->done_ = true;
	            this->on_ = false;
	        }
	    };

		constexpr bool done() const { return done_; };
		constexpr bool hit() const { return done(); };

		void stop() override
		{
			this->on_ = false;
			this->restartCounter();
			this->done_ = false;
		};

	protected:
		bool done_;
};
