#include "avoid_money_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"

AvoidMoneyGoal::AvoidMoneyGoal( std::u32string message )
:
	Goal( message )
{};

AvoidMoneyGoal::~AvoidMoneyGoal() {};

void AvoidMoneyGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( Inventory::funds() > 0 )
	{
		events.fail();
	}
};
