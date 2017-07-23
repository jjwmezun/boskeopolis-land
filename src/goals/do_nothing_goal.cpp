#include "do_nothing_goal.h"
#include "event_system.h"
#include "input.h"
#include "inventory_level.h"

DoNothingGoal::DoNothingGoal( int wait_time )
:
	Goal( "¡Don't do anything @ all!" ),
	wait_time_ ( wait_time )
{};

DoNothingGoal::~DoNothingGoal() {};

void DoNothingGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events )
{
	if
	(
		Input::pressed( Input::Action::JUMP ) ||
		Input::pressed( Input::Action::MOVE_DOWN ) ||
		Input::pressed( Input::Action::MOVE_LEFT ) ||
		Input::pressed( Input::Action::MOVE_RIGHT )
	)
	{
		events.fail();
	}

	if ( inventory.clockTime() >= wait_time_ )
	{
		events.win();
	}
};

void DoNothingGoal::customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
{
	inventory.clock().reset( Direction::Vertical::DOWN, wait_time_ );
};