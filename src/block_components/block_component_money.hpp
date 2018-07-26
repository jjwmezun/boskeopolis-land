#pragma once

#include "block_component.hpp"

class BlockComponentMoney : public BlockComponent
{
	public:
		BlockComponentMoney( int amount = 100 );
		~BlockComponentMoney();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;

	private:
		const int amount_;
};
