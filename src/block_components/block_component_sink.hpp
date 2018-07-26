#pragma once

#include "block_component.hpp"

class BlockComponentSink : public BlockComponent
{
	public:
		BlockComponentSink();
		~BlockComponentSink();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};
