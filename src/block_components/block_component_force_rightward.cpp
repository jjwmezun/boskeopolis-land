#include "block_component_force_rightward.hpp"
#include "collision.hpp"
#include "sprite.hpp"
#include <iostream>

BlockComponentForceRightward::BlockComponentForceRightward()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentForceRightward::~BlockComponentForceRightward() {};

void BlockComponentForceRightward::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	if ( sprite.vx_ < 0 )
	{
		sprite.collideStopXLeft( collision.overlapXLeft() );
	}
	else
	{
		sprite.fullStopX();
		sprite.hit_box_.x += collision.overlapXLeft();
	}
};
