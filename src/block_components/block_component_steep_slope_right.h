#pragma once

#include "block_component.h"

class BlockComponentSteepSlopeRight : public BlockComponent
{
	public:
		BlockComponentSteepSlopeRight();
		~BlockComponentSteepSlopeRight();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};