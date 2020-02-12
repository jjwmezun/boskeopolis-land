#pragma once

#include "goal.hpp"

class HeatGoal : public Goal
{
	public:
		HeatGoal( std::u32string message );
		~HeatGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
};
