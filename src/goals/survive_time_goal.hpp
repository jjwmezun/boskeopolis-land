#ifndef SURVIVE_TIME_GOAL_H
#define SURVIVE_TIME_GOAL_H

#include "goal.hpp"

class SurviveTimeGoal : public Goal
{

	public:
		SurviveTimeGoal( int wait_time = 30, std::string message = "" );
		~SurviveTimeGoal();

		void customInit( Sprite& hero, InventoryLevel& inventory_screen, EventSystem& events, Health& health ) override;
		void update( SpriteSystem& sprites, const Map& lvmap, EventSystem& events, Health& health ) override;

	private:
		const int wait_time_;
	
		const std::string surviveMessage( int wait_time, std::string message ) const;
};

#endif // SURVIVE_TIME_GOAL_H