#include "warp_goal.hpp"
#include "event_system.hpp"

WarpGoal::WarpGoal( std::u32string message )
:
	Goal( message )
{};

WarpGoal::~WarpGoal() {};

void WarpGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( events.testChangeMap() )
	{
		events.win();
	}
};