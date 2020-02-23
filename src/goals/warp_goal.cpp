#include "warp_goal.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

WarpGoal::WarpGoal( std::u32string message )
:
	Goal( message )
{};

WarpGoal::~WarpGoal() {};

void WarpGoal::update( LevelState& level_state )
{
	if ( level_state.events().testChangeMap() )
	{
		level_state.events().win();
	}
};