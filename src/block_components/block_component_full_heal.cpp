#include "block_component_full_heal.h"
#include "sprite.h"

BlockComponentFullHeal::BlockComponentFullHeal() {};

BlockComponentFullHeal::~BlockComponentFullHeal() {};

void BlockComponentFullHeal::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera )
{
	sprite.fullHeal();
};