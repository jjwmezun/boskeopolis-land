#pragma once

#include "block_component.hpp"

class BlockComponentMessage : public BlockComponent
{
	public:
		BlockComponentMessage();
		~BlockComponentMessage();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
