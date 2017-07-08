#ifndef BLOCK_COMPONENT_KEY_H
#define BLOCK_COMPONENT_KEY_H

#include "block_component.h"

class BlockComponentKey : public BlockComponent
{
	public:
		BlockComponentKey();
		~BlockComponentKey();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_KEY_H

