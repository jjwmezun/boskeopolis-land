#include "block_component_heal.hpp"
#include "sprite.hpp"

BlockComponentHeal::BlockComponentHeal( int amount ) : amount_ ( amount ) {};

BlockComponentHeal::~BlockComponentHeal() {};

void BlockComponentHeal::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	sprite.heal( amount_ );
};