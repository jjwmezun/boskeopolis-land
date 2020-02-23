#pragma once

#include "block_component.hpp"

class BlockComponentForceLeftward : public BlockComponent
{
	public:
		BlockComponentForceLeftward();
		~BlockComponentForceLeftward();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};

