#pragma once

#include "goal.h"
#include "mezun_helpers.h"

class McGuffinGoal : public Goal
{
	public:
		McGuffinGoal( int amount_needed, std::string message = mezun::emptyString() );
		~McGuffinGoal();

		void update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events, const Input& input ) override;
		void customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events ) override;

	private:
		const int amount_needed_;
		const std::string collectMessage( int amount_needed, std::string message ) const;
};
