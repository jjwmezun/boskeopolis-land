#include "past_right_edge_goal.hpp"
#include "event_system.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"

PastRightEdgeGoal::PastRightEdgeGoal( std::string message )
:
	Goal( testMessage( message ) )
{};

PastRightEdgeGoal::~PastRightEdgeGoal() {};

void PastRightEdgeGoal::update( SpriteSystem& sprites, const Map& lvmap, EventSystem& events, Health& health )
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