#include "block_component_force_upward.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentForceUpward::BlockComponentForceUpward()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentForceUpward::~BlockComponentForceUpward() {};

void BlockComponentForceUpward::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	sprite.collideStopYBottom( collision.overlapYBottom() );
};
