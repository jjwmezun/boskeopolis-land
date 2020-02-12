#pragma once

#include "goal.hpp"

class AvoidMoneyGoal : public Goal
{
	public:
		AvoidMoneyGoal( std::u32string message );
		~AvoidMoneyGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
};