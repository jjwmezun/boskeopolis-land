#ifndef DO_NOTHING_GOAL_H
#define DO_NOTHING_GOAL_H

#include "goal.hpp"

class DoNothingGoal : public Goal
{
	public:
		DoNothingGoal( int wait_time = 60 );
		~DoNothingGoal();

		void customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health );
		void update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;
		
	private:
		const int wait_time_;
};

#endif // DO_NOTHING_GOAL_H
