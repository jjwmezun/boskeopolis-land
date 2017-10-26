#pragma once

#include "block_component.hpp"

class BlockComponentMcGuffin : public BlockComponent
{
	public:
		BlockComponentMcGuffin();
		~BlockComponentMcGuffin();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};