#include "heat_goal.hpp"
#include "health.hpp"
#include "sprite_system.hpp"
#include "sprite.hpp"

HeatGoal::HeatGoal( std::u32string message )
:
	Goal( message )
{};

HeatGoal::~HeatGoal() {};

void HeatGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( sprites.hero().collidedAny() )
	{
		health.heatUp();
	}
};
