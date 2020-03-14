#pragma once

template<int rand_max, int rand_min = 0>
class TimerRandom final
{
	public:
		TimerRandom()
		:
			timer_ ( 0 ),
			target_ ( generateNewTarget() )
		{};

		bool update()
		{
			if ( timer_ >= target_ )
			{
				timer_ = 0;
				target_ = generateNewTarget();
				return true;
			}
			++timer_;
			return false;
		};

	private:
		static int generateNewTarget()
		{
			return mezun::randInt( rand_max, rand_min );
		};

		int timer_;
		int target_;
};
