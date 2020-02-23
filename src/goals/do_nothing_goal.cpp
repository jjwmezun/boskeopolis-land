#include "clock.hpp"
#include "do_nothing_goal.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level_state.hpp"

DoNothingGoal::DoNothingGoal( std::u32string message, int wait_time )
:
	Goal( message ),
	wait_time_ ( wait_time )
{};

DoNothingGoal::~DoNothingGoal() {};

void DoNothingGoal::update( LevelState& level_state )
{
	if ( Input::movingCharacter() )
	{
		level_state.events().fail();
	}

	if ( Inventory::clock().totalSeconds() >= wait_time_ )
	{
		level_state.events().win();
	}
};

void DoNothingGoal::customInit( LevelState& level_state )
{
	Inventory::clock().reset( Direction::Vertical::DOWN, wait_time_ );
};