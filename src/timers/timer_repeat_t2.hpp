#pragma once

#include "timer_t.hpp"

template<int max_T = TIMER_T_DEFAULT_FRAMES, int min_T = 0, int start_T = min_T>
class TimerRepeatT2 final
{
	public:
		constexpr TimerRepeatT2()
		:
			timer_ ( start_T )
		{};

		bool update()
		{
			if ( timer_ >= max_T )
			{
				timer_ = min_T;
				return true;
			}
            else
            {
                ++timer_;
                return false;
            }
		};

		constexpr int value() const
		{
			return timer_;
		};

		constexpr void reset()
		{
			timer_ = start_T;
		};

	private:
		int timer_;
};
