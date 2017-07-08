#include "block.h"
#include "block_component_diamond.h"
#include "collision.h"
#include "inventory_level.h"

BlockComponentDiamond::BlockComponentDiamond() {};

BlockComponentDiamond::~BlockComponentDiamond() {};

void BlockComponentDiamond::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	if ( inventory.haveDiamond( level.id() ) )
	{
		level.currentMap().deleteBlock( block.location() );
		block.destroy();
	}
	else
	{
		if ( collision.collideAny() && sprite.hasType( Sprite::SpriteType::HERO ) )
		{
			inventory.getDiamond( level.id() );
		}
	}
};
