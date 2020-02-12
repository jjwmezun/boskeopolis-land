#pragma once

#include "goal.hpp"

class McGuffinGoal : public Goal
{
	public:
		static constexpr int DEFAULT_AMOUNT_NEEDED = 3;
		McGuffinGoal( std::u32string message, int amount_needed = DEFAULT_AMOUNT_NEEDED );
		~McGuffinGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
		void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;

	private:
		const int amount_needed_;
};
