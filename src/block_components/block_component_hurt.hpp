#pragma once

#include "block_component.hpp"

class BlockComponentHurt final : public BlockComponent
{
	public:
		BlockComponentHurt();
		~BlockComponentHurt();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
