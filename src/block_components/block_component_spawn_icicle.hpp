#pragma once

#include "block_component.hpp"

class BlockComponentSpawnIcicle : public BlockComponent
{
	public:
		BlockComponentSpawnIcicle();
		~BlockComponentSpawnIcicle();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;

	private:
		int timer_;
		int limit_;
};