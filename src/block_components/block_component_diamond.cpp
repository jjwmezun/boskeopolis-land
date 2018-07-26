#include "audio.hpp"
#include "block.hpp"
#include "block_component_diamond.hpp"
#include "collision.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include <iostream>

BlockComponentDiamond::BlockComponentDiamond( int replacement_block ) : replacement_block_ ( replacement_block ) {};
BlockComponentDiamond::~BlockComponentDiamond() {};

void BlockComponentDiamond::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	if ( Inventory::haveDiamond() )
	{
		level.currentMap().deleteBlock( block.location() );

		if ( replacement_block_ == -1 )
		{
			block.destroy();
		}
		else
		{
			level.currentMap().changeBlock( block.location(), replacement_block_ );
		}
	}
	else
	{
		if ( collision.collideAny() && sprite.hasType( Sprite::SpriteType::HERO ) )
		{
			Inventory::getDiamond();
			Audio::playSound( Audio::SoundType::DIAMOND );
		}
	}
};
