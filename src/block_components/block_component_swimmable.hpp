#pragma once

#include "block_component.hpp"

class BlockComponentSwimmable final : public BlockComponent
{
	public:
		BlockComponentSwimmable();
		~BlockComponentSwimmable();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
