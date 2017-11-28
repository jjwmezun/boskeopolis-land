#include "audio.hpp"
#include "block.hpp"
#include "block_component_diamond.hpp"
#include "collision.hpp"
#include "inventory.hpp"
#include "level.hpp"

BlockComponentDiamond::BlockComponentDiamond() {};

BlockComponentDiamond::~BlockComponentDiamond() {};

void BlockComponentDiamond::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks )
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
			Audio::playSound( Audio::SoundType::DIAMOND );
		}
	}
};
