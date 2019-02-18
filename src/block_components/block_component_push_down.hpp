#pragma once

#include "block_component.hpp"

class BlockComponentPushDown : public BlockComponent
{
	public:
		BlockComponentPushDown( int force );
		~BlockComponentPushDown();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;

	private:
		const int force_;
};
