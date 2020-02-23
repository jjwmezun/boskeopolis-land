#pragma once

#include "goal.hpp"

class DoNothingGoal : public Goal
{
	public:
		DoNothingGoal( std::u32string message, int wait_time = 60 );
		~DoNothingGoal();
		void customInit( LevelState& level_state );
		void update( LevelState& level_state ) override;
		
	private:
		const int wait_time_;
};
