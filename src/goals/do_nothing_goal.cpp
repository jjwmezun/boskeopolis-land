#include "clock.hpp"
#include "do_nothing_goal.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "inventory.hpp"

DoNothingGoal::DoNothingGoal( int wait_time )
:
	Goal( "¡Don't do anything @ all!" ),
	wait_time_ ( wait_time )
{};

DoNothingGoal::~DoNothingGoal() {};

void DoNothingGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( Input::movingCharacter() )
	{
		events.fail();
	}

	if ( Inventory::clock().totalSeconds() >= wait_time_ )
	{
		events.win();
	}
};

void DoNothingGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	Inventory::clock().reset( Direction::Vertical::DOWN, wait_time_ );
};