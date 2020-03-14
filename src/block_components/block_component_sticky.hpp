#pragma once

#include "block_component.hpp"

class BlockComponentSticky final : public BlockComponent
{
	public:
		BlockComponentSticky();
		~BlockComponentSticky();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
