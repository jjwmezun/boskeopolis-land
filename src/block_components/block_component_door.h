#pragma once

#include "block_component.h"

class BlockComponentDoor : public BlockComponent
{
	public:
		BlockComponentDoor();
		~BlockComponentDoor();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

