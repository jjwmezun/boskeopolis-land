#pragma once

#include "block_component.hpp"

class BlockComponentKillAnimated : public BlockComponent
{
	public:
		BlockComponentKillAnimated();
		~BlockComponentKillAnimated();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;
};
