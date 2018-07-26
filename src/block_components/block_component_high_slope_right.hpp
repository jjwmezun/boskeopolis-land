#pragma once

#include "block_component.hpp"

class BlockComponentHighSlopeRight : public BlockComponent
{
	public:
		BlockComponentHighSlopeRight();
		~BlockComponentHighSlopeRight();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};
