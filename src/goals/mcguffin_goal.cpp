#include "mcguffin_goal.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"
#include "mezun_helpers.hpp"

McGuffinGoal::McGuffinGoal( std::u32string message, int amount_needed )
:
	Goal( formatMessage( message, amount_needed ) ),
	amount_needed_ ( amount_needed )
{};

McGuffinGoal::~McGuffinGoal() {};

void McGuffinGoal::update( LevelState& level_state )
{
	if ( level_state.inventory().mcguffins() >= amount_needed_ )
	{
		level_state.events().win();
	}
};

void McGuffinGoal::customInit( LevelState& level_state )
{
	level_state.inventory().setShowMcGuffins();
};