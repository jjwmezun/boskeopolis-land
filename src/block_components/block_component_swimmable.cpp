#include "block.hpp"
#include "block_component_swimmable.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite.hpp"

BlockComponentSwimmable::BlockComponentSwimmable() {};

BlockComponentSwimmable::~BlockComponentSwimmable() {};

void BlockComponentSwimmable::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	if ( sprite.hasType( Sprite::SpriteType::HERO ) )
	{
		if ( sprite.centerYSubPixels() > block.topSubPixels() )
		{
			level_state.health().submerge();
		}
	}

	sprite.in_water_ = true;
};
