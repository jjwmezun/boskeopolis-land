#pragma once

#include "block_component.hpp"

class BlockComponentClimbDown final : public BlockComponent
{
	public:
		BlockComponentClimbDown();
		~BlockComponentClimbDown();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
