#ifndef BLOCK_COMPONENT_FORCE_UPWARD_H
#define BLOCK_COMPONENT_FORCE_UPWARD_H

#include "block_component.h"

class BlockComponentForceUpward : public BlockComponent
{
	public:
		BlockComponentForceUpward();
		~BlockComponentForceUpward();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_FORCE_UPWARD_H
