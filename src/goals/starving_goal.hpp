#pragma once

#include "goal.hpp"
#include "timers/timer_repeat_t2.hpp"

class StarvingGoal final : public Goal
{
	public:
		static constexpr int DEFAULT_STARTING_AMOUNT = 500;
		static constexpr int DEFAULT_AMOUNT_LOST_PER_FRAME = 5;
		StarvingGoal( std::u32string message, int starting_amount = DEFAULT_STARTING_AMOUNT, int amount_lost_per_frame = DEFAULT_AMOUNT_LOST_PER_FRAME );
		~StarvingGoal();
		void customInit( LevelState& level_state ) override;
		void update( LevelState& level_state ) override;

	private:
		const int amount_lost_per_frame_;
		const int starting_amount_;
};
