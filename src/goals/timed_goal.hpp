#pragma once

#include "goal.hpp"

class TimedGoal : public Goal
{
	public:
		static constexpr int DEFAULT_TIME_LIMIT = 120;
		TimedGoal( std::u32string message, int time_limit = DEFAULT_TIME_LIMIT );
		~TimedGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
		
	private:
		const int time_limit_;
};
