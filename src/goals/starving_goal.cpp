#include "starving_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "level_state.hpp"

StarvingGoal::StarvingGoal( std::u32string message, int starting_amount, int amount_lost_per_frame )
:
	Goal( message ),
	timer_ (),
	amount_lost_per_frame_ ( amount_lost_per_frame ),
	starting_amount_ ( starting_amount )
{};

StarvingGoal::~StarvingGoal() {};

void StarvingGoal::update( LevelState& level_state )
{
	if ( timer_.update() )
	{
		Inventory::loseFunds( amount_lost_per_frame_ );
		if ( Inventory::funds() <= 0 )
		{
			level_state.events().fail();
		}
	}
};

void StarvingGoal::customInit( LevelState& level_state )
{
	Inventory::setFunds( starting_amount_ );
};