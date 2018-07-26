#pragma once

#include "block_component.hpp"

class BlockComponentSingleUse : public BlockComponent
{
	public:
		BlockComponentSingleUse( int width = 1, int height = 1, int corner = 0 );
		~BlockComponentSingleUse();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;

	private:
		const int width_;
		const int height_;
		const int corner_;
};
