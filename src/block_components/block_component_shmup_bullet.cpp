#include "audio.hpp"
#include "block.hpp"
#include "block_component_shmup_bullet.hpp"
#include "level_state.hpp"
#include "main.hpp"
#include "mezun_math.hpp"
#include "sprite_system.hpp"

BlockComponentShmupBullet::BlockComponentShmupBullet() {};

BlockComponentShmupBullet::~BlockComponentShmupBullet() {};

void BlockComponentShmupBullet::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	if ( Main::nextStateFrame( mezun::randInt( 60, 24 ) ) )
	{
		const double dx = ( double )( sprite.centerXSubPixels() - block.centerXSubPixels() );
		const double dy = ( double )( sprite.centerYSubPixels() - block.centerYSubPixels() );
		level_state.sprites().spawnShmupBullet( block.centerXPixels(), block.centerYPixels(), dy, dx );
	}
};
