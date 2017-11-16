#include "clock.hpp"
#include "timed_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "text.hpp"

TimedGoal::TimedGoal( int time_limit )
:
	Goal( "¡Don't let time run out!" ),
	time_limit_ ( time_limit )
{};

TimedGoal::~TimedGoal() {};

void TimedGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( Inventory::clock().totalSeconds() >= time_limit_ )
	{
		events.fail();
	}
};
