#pragma once

#include "goal.hpp"

class KillAllGoal : public Goal
{
	public:
		KillAllGoal( std::u32string message );
		~KillAllGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
		void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;

	private:
		int sprites_to_kill_;
		int sprites_left_;
};
