#ifndef DO_NOTHING_GOAL_H
#define DO_NOTHING_GOAL_H

#include "goal.h"

class DoNothingGoal : public Goal
{
	public:
		DoNothingGoal( int wait_time = 60 );
		~DoNothingGoal();

		void customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events );
		void update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events ) override;
		
	private:
		const int wait_time_;
};

#endif // DO_NOTHING_GOAL_H
