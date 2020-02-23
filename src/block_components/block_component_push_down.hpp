#pragma once

#include "block_component.hpp"

class BlockComponentPushDown : public BlockComponent
{
	public:
		BlockComponentPushDown( int force );
		~BlockComponentPushDown();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const int force_;
};
