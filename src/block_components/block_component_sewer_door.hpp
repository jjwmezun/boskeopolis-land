#pragma once

#include "block_component.hpp"

class BlockComponentSewerDoor : public BlockComponent
{
	public:
		BlockComponentSewerDoor();
		~BlockComponentSewerDoor();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
