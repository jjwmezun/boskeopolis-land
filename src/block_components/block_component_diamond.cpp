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
		removeDiamond( block, level.currentMap() );
	}
	else
	{
		if ( sprite.hasType( Sprite::SpriteType::HERO ) && collision.collideAny() )
		{
			Inventory::getDiamond();
			Audio::playSound( Audio::SoundType::DIAMOND );
			removeDiamond( block, level.currentMap() );
		}
	}
};

void BlockComponentDiamond::init( Block& block, Map& lvmap ) const
{
	if ( Inventory::haveDiamond() )
	{
		removeDiamond( block, lvmap );
	}
};

void BlockComponentDiamond::removeDiamond( Block& block, Map& lvmap ) const
{
	lvmap.deleteBlock( block.location() );

	if ( replacement_block_ == -1 )
	{
		block.destroy();
	}
	else
	{
		lvmap.changeBlock( block.location(), replacement_block_ );
	}
};
