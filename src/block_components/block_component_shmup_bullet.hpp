#pragma once

#include "block_component.hpp"

class BlockComponentShmupBullet : public BlockComponent
{
	public:
		BlockComponentShmupBullet();
		~BlockComponentShmupBullet();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;
};
