#include "block.hpp"
#include "block_component_diamond.hpp"
#include "collision.hpp"
#include "inventory.hpp"
#include "level.hpp"

BlockComponentDiamond::BlockComponentDiamond() {};

BlockComponentDiamond::~BlockComponentDiamond() {};

void BlockComponentDiamond::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	if ( Inventory::haveDiamond() )
	{
		level.currentMap().deleteBlock( block.location() );
		block.destroy();
	}
	else
	{
		if ( collision.collideAny() && sprite.hasType( Sprite::SpriteType::HERO ) )
		{
			Inventory::getDiamond();
		}
	}
};
