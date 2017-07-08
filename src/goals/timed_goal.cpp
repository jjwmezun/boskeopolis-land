#include "timed_goal.h"
#include "event_system.h"
#include "inventory_level.h"
#include "text.h"

TimedGoal::TimedGoal( int time_limit )
:
	Goal( "¡Don't let time run out!" ),
	time_limit_ ( time_limit )
{};

TimedGoal::~TimedGoal() {};

void TimedGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events, const Input& input )
{
	if ( inventory.clockTime() >= time_limit_ )
	{
		events.fail();
	}
};
