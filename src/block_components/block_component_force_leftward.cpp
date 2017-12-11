#include "block_component_force_leftward.hpp"
#include "collision.hpp"
#include "sprite.hpp"
#include <iostream>

BlockComponentForceLeftward::BlockComponentForceLeftward()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentForceLeftward::~BlockComponentForceLeftward() {};

void BlockComponentForceLeftward::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	if ( sprite.vx_ > 0 )
	{
		sprite.collideStopXRight( collision.overlapXRight() );
	}
	else
	{
		sprite.hit_box_.x -= collision.overlapXRight();
	}
};
