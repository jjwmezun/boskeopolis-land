#pragma once

#include "block_component.hpp"

class BlockComponentLayer : public BlockComponent
{
	public:
		BlockComponentLayer( int layer );
		~BlockComponentLayer();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;

	private:
		int layer_;
};
