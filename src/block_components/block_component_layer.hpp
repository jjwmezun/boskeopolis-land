#pragma once

#include "block_component.hpp"

class BlockComponentLayer final : public BlockComponent
{
	public:
		BlockComponentLayer( int layer );
		~BlockComponentLayer();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		int layer_;
};
