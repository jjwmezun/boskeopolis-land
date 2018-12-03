#include "kill_all_goal.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include "sprite_system.hpp"

KillAllGoal::KillAllGoal( std::string message )
:
	Goal( message ),
	sprites_to_kill_ ( 99 )
{};

KillAllGoal::~KillAllGoal() {};

void KillAllGoal::update( SpriteSystem& sprites, const Map& lvmap, InventoryLevel& inventory_screen, EventSystem& events, Health& health, LevelState& state )
{
	const int kill_count = sprites_to_kill_ - sprites.permanentlyKilledEnemies();

	inventory_screen.kill_counter_ = kill_count;

	if ( kill_count <= 0 )
	{
		events.win();
	}
};

void KillAllGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	sprites_to_kill_ = level.allEnemiesToKill();
};
