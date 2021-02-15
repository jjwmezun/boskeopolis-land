#include "kill_all_goal.hpp"
#include "event_system.hpp"
#include "inventory_level.hpp"
#include "inventory_level_graphics.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "sprite_system.hpp"

KillAllGoal::KillAllGoal( std::u32string message )
:
	Goal( message ),
	sprites_to_kill_ ( 99 ),
	sprites_left_ ( 0 )
{};

KillAllGoal::~KillAllGoal() {};

void KillAllGoal::update( LevelState& level_state )
{
	const int sprites_left_momento = sprites_left_;
	sprites_left_ = sprites_to_kill_ - level_state.sprites().permanentlyKilledEnemies();

	if ( sprites_left_ != sprites_left_momento )
	{
		level_state.inventory().changeCounter( sprites_left_ );
	}

	if ( sprites_left_ <= 0 )
	{
		level_state.events().win();
	}
};

void KillAllGoal::customInit( LevelState& level_state )
{
	sprites_left_ = sprites_to_kill_ = level_state.level().allEnemiesToKill();
	level_state.inventory().setShowCounter( InventoryLevelGraphics::Icon::COWPOKER, sprites_left_ );
};
