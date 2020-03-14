#pragma once

#include "goal.hpp"

class HeatGoal final : public Goal
{
	public:
		HeatGoal( std::u32string message );
		~HeatGoal();
		void update( LevelState& level_state ) override;
};
