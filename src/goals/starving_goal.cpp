#include "starving_goal.h"
#include "event_system.h"
#include "inventory_level.h"

StarvingGoal::StarvingGoal( int amount_lost_per_frame, int starting_amount )
:
	Goal( "¡Don't let your money run out!" ),
	delay_ ( DELAY_LENGTH ),
	amount_lost_per_frame_ ( amount_lost_per_frame ),
	starting_amount_ ( starting_amount )
{};

StarvingGoal::~StarvingGoal() {};

void StarvingGoal::update( SpriteSystem& sprites, InventoryLevel& inventory, const Map& lvmap, EventSystem& events, const Input& input )
{
	if ( delay_.hit() )
	{
		inventory.loseFunds( amount_lost_per_frame_ );

		if ( inventory.funds() <= 0 )
		{
			events.fail();
		}
	}

	delay_.update();
};

void StarvingGoal::customInit( Sprite& hero, InventoryLevel& inventory, EventSystem& events )
{
	inventory.setFunds( starting_amount_ );
};