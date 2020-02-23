#include "avoid_money_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "level_state.hpp"

AvoidMoneyGoal::AvoidMoneyGoal( std::u32string message )
:
	Goal( message )
{};

AvoidMoneyGoal::~AvoidMoneyGoal() {};

void AvoidMoneyGoal::update( LevelState& level_state )
{
	if ( Inventory::funds() > 0 )
	{
		level_state.events().fail();
	}
};
