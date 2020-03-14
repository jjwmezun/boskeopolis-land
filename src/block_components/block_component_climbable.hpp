#pragma once

#include "block_component.hpp"

class BlockComponentClimbable final : public BlockComponent
{
	public:
		BlockComponentClimbable();
		~BlockComponentClimbable();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};