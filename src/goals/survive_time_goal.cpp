#include "clock.hpp"
#include "survive_time_goal.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "mezun_helpers.hpp"
#include "text.hpp"

SurviveTimeGoal::SurviveTimeGoal( std::u32string message, int wait_time )
:
	Goal( formatMessage( message, wait_time ) ),
	wait_time_ ( wait_time )
{};

SurviveTimeGoal::~SurviveTimeGoal() {};

void SurviveTimeGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( Inventory::clock().totalSeconds() >= wait_time_ )
	{
		events.win();
	}
};

void SurviveTimeGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	Inventory::clock().reset( Direction::Vertical::DOWN, wait_time_ );
};