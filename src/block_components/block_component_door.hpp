#pragma once

#include "block_component.hpp"

class BlockComponentDoor final : public BlockComponent
{
	public:
		BlockComponentDoor();
		~BlockComponentDoor();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};

