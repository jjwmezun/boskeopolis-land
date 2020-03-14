#pragma once

#include "block_component.hpp"

class BlockComponentWarp final : public BlockComponent
{
	public:
		BlockComponentWarp();
		~BlockComponentWarp();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
