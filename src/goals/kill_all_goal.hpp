#pragma once

#include "goal.hpp"

class KillAllGoal : public Goal
{
	public:
		KillAllGoal();
		~KillAllGoal();

		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;
		void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;

	private:
		int sprites_to_kill_;
};