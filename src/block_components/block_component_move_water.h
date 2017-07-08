#ifndef BLOCK_COMPONENT_MOVE_WATER_H
#define BLOCK_COMPONENT_MOVE_WATER_H

#include "block_component.h"

class BlockComponentMoveWater : public BlockComponent
{
	public:
		BlockComponentMoveWater();
		~BlockComponentMoveWater();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_MOVE_WATER_H

