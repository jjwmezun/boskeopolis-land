#pragma once

#include "goal.hpp"

class TimedGoal : public Goal
{
	public:
		TimedGoal( int time_limit = 120 );
		~TimedGoal();

		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
		
	private:
		const int time_limit_;
};
