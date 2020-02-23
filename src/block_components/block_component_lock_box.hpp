#pragma once

#include "block_component.hpp"

class BlockComponentLockBox : public BlockComponent
{
	public:
		BlockComponentLockBox();
		~BlockComponentLockBox();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
