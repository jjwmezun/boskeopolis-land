#pragma once

#include "goal.hpp"

class AvoidMoneyGoal : public Goal
{
	public:
		AvoidMoneyGoal();
		~AvoidMoneyGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
};