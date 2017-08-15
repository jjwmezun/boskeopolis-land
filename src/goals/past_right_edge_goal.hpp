#ifndef PAST_RIGHT_EDGE_GOAL_H
#define PAST_RIGHT_EDGE_GOAL_H

#include "goal.hpp"
#include "mezun_helpers.hpp"

class PastRightEdgeGoal : public Goal
{
	public:
		PastRightEdgeGoal( std::string message = mezun::emptyString() );
		~PastRightEdgeGoal();

		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;
		
	private:
		const std::string testMessage( std::string message ) const;
};

#endif // PAST_RIGHT_EDGE_GOAL_H
