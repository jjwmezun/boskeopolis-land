#pragma once

#include "block_component.hpp"

class BlockComponentCurrent : public BlockComponent
{
	public:
		BlockComponentCurrent();
		~BlockComponentCurrent();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;
};
