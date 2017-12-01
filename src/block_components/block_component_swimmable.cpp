#include "block.hpp"
#include "block_component_swimmable.hpp"
#include "health.hpp"
#include "sprite.hpp"

BlockComponentSwimmable::BlockComponentSwimmable() {};

BlockComponentSwimmable::~BlockComponentSwimmable() {};

void BlockComponentSwimmable::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
{
	if ( sprite.hasType( Sprite::SpriteType::HERO ) )
	{
		if ( sprite.centerXSubPixels() > block.topSubPixels() )
		{
			health.submerge();
		}
	}

	sprite.in_water_ = true;
};
