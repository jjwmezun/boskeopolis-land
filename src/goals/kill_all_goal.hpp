#pragma once

#include "goal.hpp"

class KillAllGoal : public Goal
{
	public:
		KillAllGoal( std::u32string message );
		~KillAllGoal();
		void update( LevelState& level_state ) override;
		void customInit( LevelState& level_state ) override;

	private:
		int sprites_to_kill_;
		int sprites_left_;
};
