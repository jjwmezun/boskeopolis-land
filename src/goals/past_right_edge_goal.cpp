#include "past_right_edge_goal.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"

PastRightEdgeGoal::PastRightEdgeGoal( std::u32string message )
:
	Goal( message )
{};

PastRightEdgeGoal::~PastRightEdgeGoal() {};

void PastRightEdgeGoal::update( LevelState& level_state )
{
	if ( level_state.sprites().hero().rightPixels() >= level_state.currentMap().widthPixels() )
	{
		level_state.events().win();
	}
};