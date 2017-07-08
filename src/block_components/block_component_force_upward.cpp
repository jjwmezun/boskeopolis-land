#include "block_component_force_upward.h"
#include "block_type.h"
#include "collision.h"
#include "sprite.h"

BlockComponentForceUpward::BlockComponentForceUpward()
:
	BlockComponent( Type::SOLID )
{};

BlockComponentForceUpward::~BlockComponentForceUpward() {};

void BlockComponentForceUpward::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	sprite.collideStopYBottom( collision.overlapYBottom() );
};
