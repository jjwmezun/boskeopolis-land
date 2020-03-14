#pragma once

#include "goal.hpp"

class SurviveTimeGoal final : public Goal
{
	public:
		static constexpr int DEFAULT_WAIT_TIME = 60;
		SurviveTimeGoal( std::u32string message, int wait_time = DEFAULT_WAIT_TIME );
		~SurviveTimeGoal();

		void customInit( LevelState& level_state ) override;
		void update( LevelState& level_state ) override;

	private:
		const int wait_time_;
};