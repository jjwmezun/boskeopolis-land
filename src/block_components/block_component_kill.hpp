#pragma once

#include "block_component.hpp"

class BlockComponentKill final : public BlockComponent
{
	public:
		BlockComponentKill();
		~BlockComponentKill();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
