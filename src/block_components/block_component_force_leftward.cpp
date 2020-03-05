#include "block_component_force_leftward.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentForceLeftward::BlockComponentForceLeftward()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentForceLeftward::~BlockComponentForceLeftward() {};

void BlockComponentForceLeftward::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
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
