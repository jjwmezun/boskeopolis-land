#include "heat_goal.hpp"
#include "health.hpp"
#include "sprite_system.hpp"
#include "sprite.hpp"

HeatGoal::HeatGoal()
:
	Goal( "¡Don't touch the hot surface too long!" )
{};

HeatGoal::~HeatGoal() {};

void HeatGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( sprites.hero().collidedAny() )
	{
		health.heatUp();
	}
};
