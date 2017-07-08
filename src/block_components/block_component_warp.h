#ifndef BLOCK_COMPONENT_WARP_H
#define BLOCK_COMPONENT_WARP_H

#include "block_component.h"

class BlockComponentWarp : public BlockComponent
{
	public:
		BlockComponentWarp();
		~BlockComponentWarp();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_WARP_H
