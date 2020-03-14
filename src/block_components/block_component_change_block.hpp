#pragma once

#include "block_component.hpp"

class BlockComponentChangeBlock final : public BlockComponent
{
	public:
		BlockComponentChangeBlock( int new_block );
		~BlockComponentChangeBlock();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const int new_block_;
};