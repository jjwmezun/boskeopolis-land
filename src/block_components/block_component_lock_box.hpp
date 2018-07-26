#pragma once

#include "block_component.hpp"

class BlockComponentLockBox : public BlockComponent
{
	public:
		BlockComponentLockBox();
		~BlockComponentLockBox();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;
};
