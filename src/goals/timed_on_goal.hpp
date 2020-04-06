#pragma once

#include "goal.hpp"

class TimedOnGoal final : public Goal
{
	public:
		TimedOnGoal( std::u32string message, int time_limit_seconds = 10 );
		~TimedOnGoal();
		void update( LevelState& level_state ) override;
		
	private:
		const int time_limit_frames_;
        int current_time_;
};
