#include "starving_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"

StarvingGoal::StarvingGoal( int amount_lost_per_frame, int starting_amount )
:
	Goal( "¡Don't let your money run out!" ),
	delay_ ( DELAY_LENGTH ),
	amount_lost_per_frame_ ( amount_lost_per_frame ),
	starting_amount_ ( starting_amount )
{};

StarvingGoal::~StarvingGoal() {};

void StarvingGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	if ( delay_.hit() )
	{
		Inventory::loseFunds( amount_lost_per_frame_ );

		if ( Inventory::funds() <= 0 )
		{
			events.fail();
		}
	}

	delay_.update();
};

void StarvingGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	Inventory::setFunds( starting_amount_ );
};