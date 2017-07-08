#ifndef BLOCK_COMPONENT_HURT_H
#define BLOCK_COMPONENT_HURT_H

#include "block_component.h"

class BlockComponentHurt : public BlockComponent
{
	public:
		BlockComponentHurt();
		~BlockComponentHurt();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_HURT_H

