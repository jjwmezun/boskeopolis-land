#include "collect_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "text.hpp"

CollectGoal::CollectGoal( int amount_needed, std::string message )
:
	Goal( collectMessage( amount_needed, message ) ),
	amount_needed_ ( amount_needed )
{};

CollectGoal::~CollectGoal() {};

void CollectGoal::update( SpriteSystem& sprites, const Map& lvmap, EventSystem& events, Health& health )
{
	if ( Inventory::funds() >= amount_needed_ )
	{
		events.win();
	}
};

const std::string CollectGoal::collectMessage( int amount_needed, std::string message ) const
{
	if ( mezun::isStringEmpty( message ) )
	{
		// Default message
		return ( std::string )"¡Collect " + Text::formatNumCommas( Text::stringifyNum( amount_needed ) ) + (std::string)"\t!";
	}
	else
	{
		return message;
	}
};