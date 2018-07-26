#pragma once

#include "block_component.hpp"

class BlockComponentLowSlopeRight : public BlockComponent
{
	public:
		BlockComponentLowSlopeRight();
		~BlockComponentLowSlopeRight();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};
