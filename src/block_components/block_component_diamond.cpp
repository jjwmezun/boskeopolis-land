#include "block.hpp"
#include "block_component_diamond.hpp"
#include "collision.hpp"
#include "inventory_level.hpp"

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
