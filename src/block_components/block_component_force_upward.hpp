#pragma once

#include "block_component.hpp"

class BlockComponentForceUpward : public BlockComponent
{
	public:
		BlockComponentForceUpward();
		~BlockComponentForceUpward();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};

