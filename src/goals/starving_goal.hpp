#pragma once

#include "goal.hpp"
#include "timers/timer_repeat_t2.hpp"

class StarvingGoal : public Goal
{
	public:
		static constexpr int DEFAULT_STARTING_AMOUNT = 500;
		static constexpr int DEFAULT_AMOUNT_LOST_PER_FRAME = 50;
		StarvingGoal( std::u32string message, int starting_amount = DEFAULT_STARTING_AMOUNT, int amount_lost_per_frame = DEFAULT_AMOUNT_LOST_PER_FRAME );
		~StarvingGoal();
		void customInit( LevelState& level_state ) override;
		void update( LevelState& level_state ) override;

	private:
		static constexpr int DELAY_LENGTH = 16;
	
		TimerRepeatT2<DELAY_LENGTH> timer_;
		const int amount_lost_per_frame_;
		const int starting_amount_;
};
