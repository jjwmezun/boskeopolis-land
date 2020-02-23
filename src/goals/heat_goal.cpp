#include "heat_goal.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_system.hpp"
#include "sprite.hpp"

HeatGoal::HeatGoal( std::u32string message )
:
	Goal( message )
{};

HeatGoal::~HeatGoal() {};

void HeatGoal::update( LevelState& level_state )
{
	if ( level_state.sprites().hero().collidedAny() )
	{
		level_state.health().heatUp();
	}
};
