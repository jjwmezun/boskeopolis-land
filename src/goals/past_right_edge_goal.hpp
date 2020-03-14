#pragma once

#include "goal.hpp"

class PastRightEdgeGoal final : public Goal
{
	public:
		PastRightEdgeGoal( std::u32string message );
		~PastRightEdgeGoal();
		void update( LevelState& level_state ) override;
};
