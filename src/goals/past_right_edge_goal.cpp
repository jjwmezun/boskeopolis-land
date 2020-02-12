#include "past_right_edge_goal.hpp"
#include "event_system.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"

PastRightEdgeGoal::PastRightEdgeGoal( std::u32string message )
:
	Goal( message )
{};

PastRightEdgeGoal::~PastRightEdgeGoal() {};

void PastRightEdgeGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	if ( sprites.hero().xPixels() > lvmap.widthPixels() + sprites.hero().widthPixels() )
	{
		events.win();
	}
};