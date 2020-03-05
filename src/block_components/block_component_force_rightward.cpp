#include "block_component_force_rightward.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentForceRightward::BlockComponentForceRightward()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentForceRightward::~BlockComponentForceRightward() {};

void BlockComponentForceRightward::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
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
