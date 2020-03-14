#pragma once

#include "block_component.hpp"

class BlockComponentKillAnimated final : public BlockComponent
{
	public:
		BlockComponentKillAnimated();
		~BlockComponentKillAnimated();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
