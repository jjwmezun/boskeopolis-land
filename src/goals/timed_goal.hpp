#pragma once

#include "goal.hpp"

class TimedGoal final : public Goal
{
	public:
		static constexpr int DEFAULT_TIME_LIMIT = 120;
		TimedGoal( std::u32string message, int time_limit = DEFAULT_TIME_LIMIT );
		~TimedGoal();
		void update( LevelState& level_state ) override;
		void customInit( LevelState& level_state ) override;
		
	private:
		const int time_limit_;
};
