#ifndef AVOID_MONEY_GOAL_H
#define AVOID_MONEY_GOAL_H

#include "goal.hpp"

class AvoidMoneyGoal : public Goal
{

	public:
		AvoidMoneyGoal();
		~AvoidMoneyGoal();

		void update( SpriteSystem& sprites, const Map& lvmap, EventSystem& events ) override;
};

#endif // AVOID_MONEY_GOAL_H
