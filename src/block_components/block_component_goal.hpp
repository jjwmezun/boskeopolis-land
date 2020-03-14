#pragma once

#include "block_component.hpp"

class BlockComponentGoal final : public BlockComponent
{
	public:
		BlockComponentGoal();
		~BlockComponentGoal();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
