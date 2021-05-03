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
	if ( level_state.inventory().clock().countdownHit0() )
	{
		level_state.events().fail();
	}
};

void TimedGoal::customInit( LevelState& level_state )
{
	level_state.inventory().clock().reset( Direction::Vertical::DOWN, time_limit_ );
}
