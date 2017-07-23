#include "past_right_edge_goal.h"
#include "event_system.h"
#include "map.h"
#include "sprite.h"
#include "sprite_system.h"

PastRightEdgeGoal::PastRightEdgeGoal( std::string message )
:
	Goal( testMessage( message ) )
{};

PastRightEdgeGoal::~PastRightEdgeGoal() {};

void PastRightEdgeGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events )
{
	if ( sprites.hero().xPixels() > lvmap.widthPixels() + sprites.hero().widthPixels() )
	{
		events.win();
	}
};

const std::string PastRightEdgeGoal::testMessage( std::string message ) const
{
	if ( mezun::isStringEmpty( message ) )
	{
		// Default message
		return "¡Go past the end!";
	}
	else
	{
		return message;
	}
};