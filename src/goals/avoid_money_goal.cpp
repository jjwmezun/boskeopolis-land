#include "avoid_money_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"

AvoidMoneyGoal::AvoidMoneyGoal()
:
	Goal( "¡Don't touch any money!" )
{};

AvoidMoneyGoal::~AvoidMoneyGoal() {};

void AvoidMoneyGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( Inventory::funds() > 0 )
	{
		events.fail();
	}
};
