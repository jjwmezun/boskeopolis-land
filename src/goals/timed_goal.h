#ifndef TIMED_GOAL_H
#define TIMED_GOAL_H

#include "goal.h"

class TimedGoal : public Goal
{
	public:
		TimedGoal( int time_limit = 120 );
		~TimedGoal();

		void update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events ) override;
		
	private:
		const int time_limit_;
};

#endif // TIMED_GOAL_H
