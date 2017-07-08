#include "collect_goal.h"
#include "event_system.h"
#include "inventory_level.h"
#include "text.h"

CollectGoal::CollectGoal( int amount_needed, std::string message )
:
	Goal( collectMessage( amount_needed, message ) ),
	amount_needed_ ( amount_needed )
{};

CollectGoal::~CollectGoal() {};

void CollectGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events, const Input& input )
{
	if ( inventory.funds() >= amount_needed_ )
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