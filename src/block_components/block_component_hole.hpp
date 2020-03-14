#pragma once

#include "block_component.hpp"

class BlockComponentHole final : public BlockComponent
{
	public:
		BlockComponentHole();
		~BlockComponentHole();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
