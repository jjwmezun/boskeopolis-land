#include "block_component_force_upward.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentForceUpward::BlockComponentForceUpward()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentForceUpward::~BlockComponentForceUpward() {};

void BlockComponentForceUpward::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	sprite.collideStopYBottom( collision.overlapYBottom() );
};
