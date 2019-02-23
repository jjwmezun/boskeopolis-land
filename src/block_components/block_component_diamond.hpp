#pragma once

#include "block_component.hpp"

class BlockComponentDiamond : public BlockComponent
{
	public:
		BlockComponentDiamond( int replacement_block = -1 );
		~BlockComponentDiamond();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;
		void init( Block& block, Map& lvmap ) const override;

	private:
		const int replacement_block_;
		void removeDiamond( Block& block, Map& lvmap ) const;
};
