#ifndef SURVIVE_TIME_GOAL_H
#define SURVIVE_TIME_GOAL_H

#include "goal.hpp"

class SurviveTimeGoal : public Goal
{

	public:
		SurviveTimeGoal( int wait_time = 30, std::string message = "" );
		~SurviveTimeGoal();

		void customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events ) override;
		void update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events ) override;

	private:
		const int wait_time_;
	
		const std::string surviveMessage( int wait_time, std::string message ) const;
};

#endif // SURVIVE_TIME_GOAL_H