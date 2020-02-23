#pragma once

#include "goal.hpp"

class AvoidMoneyGoal : public Goal
{
	public:
		AvoidMoneyGoal( std::u32string message );
		~AvoidMoneyGoal();
		void update( LevelState& level_state ) override;
};