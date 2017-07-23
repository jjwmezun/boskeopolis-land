#ifndef WINDY_GOAL_H
#define WINDY_GOAL_H

#include "direction.hpp"
#include "goal.hpp"
#include "sprite.hpp"

class WindyGoal : public Goal
{

	public:
		WindyGoal( int wind_strength, Direction::Horizontal direction = Direction::Horizontal::__NULL );
		~WindyGoal();

		void customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events ) override;

	private:
		const int wind_strength_;

		int windStrength( Direction::Horizontal direction, unsigned int wind_strength );
};

#endif // WINDY_GOAL_H
