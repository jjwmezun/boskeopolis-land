#ifndef BLOCK_COMPONENT_LOW_SLOPE_LEFT_H
#define BLOCK_COMPONENT_LOW_SLOPE_LEFT_H

#include "block_component.h"

class BlockComponentLowSlopeLeft : public BlockComponent
{
	public:
		BlockComponentLowSlopeLeft();
		~BlockComponentLowSlopeLeft();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_LOW_SLOPE_LEFT_H

