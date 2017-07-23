#include "block.hpp"
#include "block_component_swimmable.hpp"
#include "sprite.hpp"

BlockComponentSwimmable::BlockComponentSwimmable() {};

BlockComponentSwimmable::~BlockComponentSwimmable() {};

void BlockComponentSwimmable::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera )
{
	if ( sprite.centerXSubPixels() > block.topSubPixels() )
	{
		sprite.submerged_in_water_ = true;
	}

	sprite.swim();
};
