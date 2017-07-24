#ifndef STARVING_GOAL_H
#define STARVING_GOAL_H

#include "goal.hpp"
#include "timers/timer_repeat.hpp"

class StarvingGoal : public Goal
{

	public:
		StarvingGoal( int amount_lost_per_frame = 50, int starting_amount = 500 );
		~StarvingGoal();

		void customInit( Sprite& hero, InventoryLevel& inventory_screen, EventSystem& events ) override;
		void update( SpriteSystem& sprites, const Map& lvmap, EventSystem& events ) override;

	private:
		static constexpr int DELAY_LENGTH = 16;
	
		TimerRepeat delay_;
		const int amount_lost_per_frame_;
		const int starting_amount_;
};

#endif // STARVING_GOAL_H