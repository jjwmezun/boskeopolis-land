#include "mcguffin_goal.h"
#include "event_system.h"
#include "inventory_level.h"
#include "text.h"

McGuffinGoal::McGuffinGoal( int amount_needed, std::string message )
:
	Goal( collectMessage( amount_needed, message ) ),
	amount_needed_ ( amount_needed )
{};

McGuffinGoal::~McGuffinGoal() {};

void McGuffinGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events )
{
	if ( inventory.McGuffins() >= amount_needed_ )
	{
		events.win();
	}
};

void McGuffinGoal::customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
{
	inventory.show_mcguffins_ = true;
};

const std::string McGuffinGoal::collectMessage( int amount_needed, std::string message ) const
{
	if ( mezun::isStringEmpty( message ) )
	{
		// Default message
		return ( std::string )"¡Collect " + Text::formatNumCommas( Text::stringifyNum( amount_needed ) ) + (std::string)" McGuffins!";
	}
	else
	{
		return message;
	}
};