#pragma once

#include "goal.h"
#include "mezun_helpers.h"

class CollectGoal : public Goal
{
	public:
		CollectGoal( int amount_needed, std::string message = mezun::emptyString() );
		~CollectGoal();

		void update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events ) override;

	private:
		const int amount_needed_;
		
		const std::string collectMessage( int amount_needed, std::string message ) const;
};
