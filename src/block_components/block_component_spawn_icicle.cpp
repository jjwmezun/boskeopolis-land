#include "block.hpp"
#include "block_component_spawn_icicle.hpp"
#include "main.hpp"
#include "mezun_math.hpp"
#include "sprite_system.hpp"

#include <iostream>

int makeLimit()
{
	return mezun::randInt( 120, 80 );
};

BlockComponentSpawnIcicle::BlockComponentSpawnIcicle() : timer_ ( 0 ), limit_ ( makeLimit() ) {};

BlockComponentSpawnIcicle::~BlockComponentSpawnIcicle() {};

void BlockComponentSpawnIcicle::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	if ( timer_ == limit_ )
	{
		sprites.spawnIcicle( block.xPixels(), block.yPixels() );
		timer_ = 0;
		limit_ = makeLimit();
	}
	else
	{
		++timer_;
	}
};