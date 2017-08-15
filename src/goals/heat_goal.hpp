#pragma once

#include "goal.hpp"

class HeatGoal : public Goal
{
	public:
		HeatGoal();
		~HeatGoal();

		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;
};
