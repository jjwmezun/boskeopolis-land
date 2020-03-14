#pragma once

#include "goal.hpp"

class WarpGoal final : public Goal
{
	public:
		WarpGoal( std::u32string message );
		~WarpGoal();
		void update( LevelState& level_state ) override;
};