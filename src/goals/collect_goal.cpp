#include "collect_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "level_state.hpp"

CollectGoal::CollectGoal( std::u32string message, int amount_needed )
:
	Goal( formatMessage( message, amount_needed ) ),
	amount_needed_ ( amount_needed )
{};

CollectGoal::~CollectGoal() {};

void CollectGoal::update( LevelState& level_state )
{
	if ( Inventory::funds() >= amount_needed_ )
	{
		level_state.events().win();
	}
};