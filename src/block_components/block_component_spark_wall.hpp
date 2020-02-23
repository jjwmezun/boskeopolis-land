#pragma once

#include "block_component.hpp"

class BlockComponentSparkWall : public BlockComponent
{
	public:
		BlockComponentSparkWall();
		~BlockComponentSparkWall();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
