#include "clock.hpp"
#include "timed_goal.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"

TimedGoal::TimedGoal( std::u32string message, int time_limit )
:
	Goal( message ),
	time_limit_ ( time_limit )
{};

TimedGoal::~TimedGoal() {};

void TimedGoal::update( LevelState& level_state )
{
	if ( level_state.inventory().clock().totalSeconds() >= time_limit_ )
	{
		level_state.events().fail();
	}
};
