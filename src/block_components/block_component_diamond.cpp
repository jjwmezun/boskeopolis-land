#include "audio.hpp"
#include "block.hpp"
#include "block_component_diamond.hpp"
#include "collision.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include "level_state.hpp"

BlockComponentDiamond::BlockComponentDiamond( int replacement_block )
:
	BlockComponent( Type::DIAMOND ),
	replacement_block_ ( replacement_block )
{};
BlockComponentDiamond::~BlockComponentDiamond() {};

void BlockComponentDiamond::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	if ( Inventory::haveDiamond() )
	{
		removeDiamond( block, level_state );
	}
	else
	{
		if ( sprite.hasType( Sprite::SpriteType::HERO ) && collision.collideAny() )
		{
			Inventory::getDiamond();
			Audio::playSound( Audio::SoundType::DIAMOND );
			removeDiamond( block, level_state );
			level_state.inventory().setDiamondGotten();
		}
	}
};

void BlockComponentDiamond::init( Block& block, LevelState& level_state ) const
{
	if ( Inventory::haveDiamond() )
	{
		removeDiamond( block, level_state );
	}
};

void BlockComponentDiamond::removeDiamond( Block& block, LevelState& level_state ) const
{
	level_state.currentMap().deleteBlock( block.layer(), block.location() );

	if ( replacement_block_ == -1 )
	{
		block.destroy();
	}
	else
	{
		level_state.currentMap().changeBlock( block.layer(), block.location(), replacement_block_ );
	}
};
