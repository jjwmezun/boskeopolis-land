#pragma once

#include "block_component.hpp"

class BlockComponentSolid final : public BlockComponent
{
	public:
		BlockComponentSolid();
		~BlockComponentSolid();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
