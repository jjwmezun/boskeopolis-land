#pragma once

#include "goal.hpp"

class StopOnOffGoal final : public Goal
{
	public:
		StopOnOffGoal( std::u32string message );
		~StopOnOffGoal();
		void update( LevelState& level_state ) override;
		void customInit( LevelState& level_state ) override;
};