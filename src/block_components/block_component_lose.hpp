#pragma once

#include "block_component.hpp"

class BlockComponentLose final : public BlockComponent
{
	public:
		BlockComponentLose();
		~BlockComponentLose();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
