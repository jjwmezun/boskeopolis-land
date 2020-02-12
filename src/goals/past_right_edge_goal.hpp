#pragma once

#include "goal.hpp"

class PastRightEdgeGoal : public Goal
{
	public:
		PastRightEdgeGoal( std::u32string message );
		~PastRightEdgeGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
};
