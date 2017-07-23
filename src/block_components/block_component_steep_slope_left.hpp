#pragma once

#include "block_component.hpp"

class BlockComponentSteepSlopeLeft : public BlockComponent
{
	public:
		BlockComponentSteepSlopeLeft();
		~BlockComponentSteepSlopeLeft();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};