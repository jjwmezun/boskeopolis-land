#pragma once

#include "goal.hpp"
#include "mezun_helpers.hpp"

class McGuffinGoal : public Goal
{
	public:
		McGuffinGoal( int amount_needed, std::string message = mezun::emptyString() );
		~McGuffinGoal();

		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state ) override;
		void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;

	private:
		const int amount_needed_;
		const std::string collectMessage( int amount_needed, std::string message ) const;
};
