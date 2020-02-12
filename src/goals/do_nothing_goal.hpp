#pragma once

#include "goal.hpp"

class DoNothingGoal : public Goal
{
	public:
		DoNothingGoal( std::u32string message, int wait_time = 60 );
		~DoNothingGoal();
		void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health );
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
		
	private:
		const int wait_time_;
};
