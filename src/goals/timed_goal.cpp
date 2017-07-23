#include "timed_goal.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "text.hpp"

TimedGoal::TimedGoal( int time_limit )
:
	Goal( "¡Don't let time run out!" ),
	time_limit_ ( time_limit )
{};

TimedGoal::~TimedGoal() {};

void TimedGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events )
{
	if ( inventory.clockTime() >= time_limit_ )
	{
		events.fail();
	}
};
