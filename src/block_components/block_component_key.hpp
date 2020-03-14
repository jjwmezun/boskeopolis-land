#pragma once

#include "block_component.hpp"

class BlockComponentKey final : public BlockComponent
{
	public:
		BlockComponentKey();
		~BlockComponentKey();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};

