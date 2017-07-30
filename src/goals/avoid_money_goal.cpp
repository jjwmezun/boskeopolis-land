#include "avoid_money_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"

AvoidMoneyGoal::AvoidMoneyGoal()
:
	Goal( "¡Don't touch any money!" )
{};

AvoidMoneyGoal::~AvoidMoneyGoal() {};

void AvoidMoneyGoal::update( SpriteSystem& sprites, const Map& lvmap, EventSystem& events, Health& health )
{
	if ( Inventory::funds() > 0 )
	{
		events.fail();
	}
};
