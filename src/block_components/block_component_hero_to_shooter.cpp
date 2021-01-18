#include "block_component_hero_to_shooter.hpp"
#include "level_state.hpp"
#include "sprite_system.hpp"

BlockComponentHeroToShooter::BlockComponentHeroToShooter() {};

BlockComponentHeroToShooter::~BlockComponentHeroToShooter() {};

void BlockComponentHeroToShooter::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.sprites().changeHeroToShooter();
};