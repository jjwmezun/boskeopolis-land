#pragma once

#include "block_component.hpp"

class BlockComponentBouncy : public BlockComponent
{
	public:
		BlockComponentBouncy( int strength = 24 );
		~BlockComponentBouncy();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;

	private:
		const int strength_;
};