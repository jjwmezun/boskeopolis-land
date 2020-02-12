#pragma once

#include "goal.hpp"

class CollectGoal : public Goal
{
	public:
		static constexpr int DEFAULT_AMOUNT = 10000;
		CollectGoal( std::u32string message, int amount_needed = DEFAULT_AMOUNT );
		~CollectGoal();
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;

	private:
		const int amount_needed_;
};
