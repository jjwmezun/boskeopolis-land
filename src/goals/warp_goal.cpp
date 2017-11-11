#include "warp_goal.hpp"
#include "event_system.hpp"

WarpGoal::WarpGoal( const std::string& msg )
:
	Goal( msg )
{};

WarpGoal::~WarpGoal() {};

void WarpGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	if ( events.change_map_ )
	{
		events.win();
	}
};