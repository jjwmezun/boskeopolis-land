#pragma once

template<int max_T, int min_T = 0>
class TimerRepeatT2
{
	public:
		TimerRepeatT2()
		:
			timer_ ( min_T )
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

	private:
		int timer_;
};
