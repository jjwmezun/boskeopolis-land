#pragma once

#include "block_component.hpp"

class BlockComponentDiamond : public BlockComponent
{
	public:
		BlockComponentDiamond( int replacement_block = -1 );
		~BlockComponentDiamond();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
		void init( Block& block, LevelState& level_state ) const override;

	private:
		const int replacement_block_;
		void removeDiamond( Block& block, LevelState& level_state ) const;
};
