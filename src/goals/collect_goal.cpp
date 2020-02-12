#include "collect_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "mezun_helpers.hpp"

CollectGoal::CollectGoal( std::u32string message, int amount_needed )
:
	Goal( formatMessage( message, amount_needed ) ),
	amount_needed_ ( amount_needed )
{};

CollectGoal::~CollectGoal() {};

void CollectGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( Inventory::funds() >= amount_needed_ )
	{
		events.win();
	}
};