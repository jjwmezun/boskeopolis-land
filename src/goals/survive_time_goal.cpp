#include "survive_time_goal.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "mezun_helpers.hpp"
#include "text.hpp"

SurviveTimeGoal::SurviveTimeGoal( int wait_time, std::string message )
:
	Goal( surviveMessage( wait_time, message ) ),
	wait_time_ ( wait_time )
{};

SurviveTimeGoal::~SurviveTimeGoal() {};

void SurviveTimeGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events )
{
	if ( inventory.clockTime() >= wait_time_ )
	{
		events.win();
	}
};

void SurviveTimeGoal::customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
{
	inventory.clock().reset( Direction::Vertical::DOWN, wait_time_ );
};

const std::string SurviveTimeGoal::surviveMessage( int wait_time, std::string message ) const
{
	if ( mezun::isStringEmpty( message ) )
	{
		// Default message
		return ( std::string )"¡Last " + Text::formatNumCommas( Text::stringifyNum( wait_time ) ) + (std::string)" seconds without dying!";
	}
	else
	{
		return message;
	}
};