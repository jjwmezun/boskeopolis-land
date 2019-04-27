#pragma once

#include "block_component.hpp"

class BlockComponentHole : public BlockComponent
{
	public:
		BlockComponentHole();
		~BlockComponentHole();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;
};
