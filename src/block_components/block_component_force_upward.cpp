#include "block_component_force_upward.hpp"
#include "block_type.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentForceUpward::BlockComponentForceUpward()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentForceUpward::~BlockComponentForceUpward() {};

void BlockComponentForceUpward::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera )
{
	sprite.collideStopYBottom( collision.overlapYBottom() );
};
