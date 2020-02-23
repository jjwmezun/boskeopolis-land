#pragma once

#include "block_component.hpp"

class BlockComponentMcGuffin : public BlockComponent
{
	public:
		BlockComponentMcGuffin();
		~BlockComponentMcGuffin();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};