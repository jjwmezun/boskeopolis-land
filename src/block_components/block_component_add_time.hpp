#pragma once

#include "block_component.hpp"

class BlockComponentAddTime final : public BlockComponent
{
	public:
		BlockComponentAddTime( int seconds );
		~BlockComponentAddTime();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const int seconds_;
};

