#pragma once

#include "block_component.h"

class BlockComponentMcGuffin : public BlockComponent
{
	public:
		BlockComponentMcGuffin();
		~BlockComponentMcGuffin();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};