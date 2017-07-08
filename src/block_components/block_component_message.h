#ifndef BLOCK_COMPONENT_MESSAGE_H
#define BLOCK_COMPONENT_MESSAGE_H

#include "block_component.h"

class BlockComponentMessage : public BlockComponent
{
	public:
		BlockComponentMessage();
		~BlockComponentMessage();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_MESSAGE_H

