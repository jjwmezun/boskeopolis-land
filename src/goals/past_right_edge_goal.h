#ifndef PAST_RIGHT_EDGE_GOAL_H
#define PAST_RIGHT_EDGE_GOAL_H

#include "goal.h"
#include "mezun_helpers.h"

class PastRightEdgeGoal : public Goal
{
	public:
		PastRightEdgeGoal( std::string message = mezun::emptyString() );
		~PastRightEdgeGoal();

		void update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events ) override;
		
	private:
		const std::string testMessage( std::string message ) const;
};

#endif // PAST_RIGHT_EDGE_GOAL_H
