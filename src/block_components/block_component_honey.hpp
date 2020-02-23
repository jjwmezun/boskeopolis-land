#pragma once

#include "block_component.hpp"

class BlockComponentHoney : public BlockComponent
{
	public:
		BlockComponentHoney();
		~BlockComponentHoney();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
