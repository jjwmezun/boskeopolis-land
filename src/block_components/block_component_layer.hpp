#pragma once

#include "block_component.hpp"
#include "unit.hpp"

class BlockComponentLayer final : public BlockComponent
{
	public:
		BlockComponentLayer( Unit::Layer layer );
		~BlockComponentLayer();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		Unit::Layer layer_;
};
