#ifndef BLOCK_COMPONENT_SWIMMABLE_H
#define BLOCK_COMPONENT_SWIMMABLE_H

#include "block_component.h"

class BlockComponentSwimmable : public BlockComponent
{
	public:
		BlockComponentSwimmable();
		~BlockComponentSwimmable();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_SWIMMABLE_H

