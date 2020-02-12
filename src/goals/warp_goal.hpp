#pragma once

#include "goal.hpp"

class WarpGoal : public Goal
{
	public:
		WarpGoal( std::u32string message );
		~WarpGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
};