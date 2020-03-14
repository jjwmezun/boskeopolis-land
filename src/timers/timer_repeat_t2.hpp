#pragma once

template<int max_T, int min_T = 0>
class TimerRepeatT2 final
{
	public:
		constexpr TimerRepeatT2()
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

		constexpr int value() const
		{
			return timer_;
		};

	private:
		int timer_;
};
