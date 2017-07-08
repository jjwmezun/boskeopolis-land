#ifndef BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H
#define BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H

#include "block_component.h"

class BlockComponentLowSlopeRight : public BlockComponent
{
	public:
		BlockComponentLowSlopeRight();
		~BlockComponentLowSlopeRight();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H

