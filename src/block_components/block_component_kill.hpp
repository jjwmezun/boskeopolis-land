#pragma once

#include "block_component.hpp"

class BlockComponentKill : public BlockComponent
{
	public:
		BlockComponentKill();
		~BlockComponentKill();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};
