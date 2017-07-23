#ifndef BLOCK_COMPONENT_HIGH_SLOPE_RIGHT_H
#define BLOCK_COMPONENT_HIGH_SLOPE_RIGHT_H

#include "block_component.hpp"

class BlockComponentHighSlopeRight : public BlockComponent
{
	public:
		BlockComponentHighSlopeRight();
		~BlockComponentHighSlopeRight();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_HIGH_SLOPE_RIGHT_H

