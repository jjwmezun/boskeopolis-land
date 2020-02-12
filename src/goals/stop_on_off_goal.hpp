#pragma once

#include "goal.hpp"

class StopOnOffGoal : public Goal
{
	public:
		StopOnOffGoal( std::u32string message );
		~StopOnOffGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
		void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;
};