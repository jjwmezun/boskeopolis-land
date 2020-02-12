#pragma once

#include "goal.hpp"

class SurviveTimeGoal : public Goal
{
	public:
		static constexpr int DEFAULT_WAIT_TIME = 60;
		SurviveTimeGoal( std::u32string message, int wait_time = DEFAULT_WAIT_TIME );
		~SurviveTimeGoal();

		void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;

	private:
		const int wait_time_;
};