#include "avoid_money_goal.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"

AvoidMoneyGoal::AvoidMoneyGoal()
:
	Goal( "¡Don't touch any money!" )
{};

AvoidMoneyGoal::~AvoidMoneyGoal() {};

void AvoidMoneyGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events )
{
	if ( inventory.funds() > 0 )
	{
		events.fail();
	}
};
