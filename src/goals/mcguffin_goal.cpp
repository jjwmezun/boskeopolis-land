#include "mcguffin_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "text.hpp"

McGuffinGoal::McGuffinGoal( int amount_needed, std::string message )
:
	Goal( collectMessage( amount_needed, message ) ),
	amount_needed_ ( amount_needed )
{};

McGuffinGoal::~McGuffinGoal() {};

void McGuffinGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( Inventory::McGuffins() >= amount_needed_ )
	{
		events.win();
	}
};

void McGuffinGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	inventory_screen.show_mcguffins_ = true;
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