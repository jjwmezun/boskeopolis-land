#include "starving_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"

StarvingGoal::StarvingGoal( std::u32string message, int starting_amount, int amount_lost_per_frame )
:
	Goal( message ),
	timer_ (),
	amount_lost_per_frame_ ( amount_lost_per_frame ),
	starting_amount_ ( starting_amount )
{};

StarvingGoal::~StarvingGoal() {};

void StarvingGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( timer_.update() )
	{
		Inventory::loseFunds( amount_lost_per_frame_ );
		if ( Inventory::funds() <= 0 )
		{
			events.fail();
		}
	}
};

void StarvingGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	Inventory::setFunds( starting_amount_ );
};