#pragma once

#include "block_component.hpp"

class BlockComponentSteepSlopeLeft : public BlockComponent
{
	public:
		BlockComponentSteepSlopeLeft();
		~BlockComponentSteepSlopeLeft();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};