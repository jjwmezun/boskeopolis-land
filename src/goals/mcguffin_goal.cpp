#include "mcguffin_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "mezun_helpers.hpp"

McGuffinGoal::McGuffinGoal( std::u32string message, int amount_needed )
:
	Goal( formatMessage( message, amount_needed ) ),
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
	inventory_screen.setShowMcGuffins();
};