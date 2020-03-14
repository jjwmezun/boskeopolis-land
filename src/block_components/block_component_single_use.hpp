#pragma once

#include "block_component.hpp"

class BlockComponentSingleUse final : public BlockComponent
{
	public:
		BlockComponentSingleUse( int width = 1, int height = 1, int corner = 0 );
		~BlockComponentSingleUse();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const int width_;
		const int height_;
		const int corner_;
};
