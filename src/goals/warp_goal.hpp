#pragma once

#include "goal.hpp"

class WarpGoal : public Goal
{
	public:
		WarpGoal( const std::string& msg = "¡Get out o' here!" );
		~WarpGoal();

		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
};