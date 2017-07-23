#include "avoid_money_goal.h"
#include "event_system.h"
#include "inventory_level.h"

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
