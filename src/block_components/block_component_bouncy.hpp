#pragma once

#include "block_component.hpp"

class BlockComponentBouncy : public BlockComponent
{
	public:
		BlockComponentBouncy( int strength = 24 );
		~BlockComponentBouncy();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const int strength_;
};