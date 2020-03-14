#pragma once

#include "block_component.hpp"
#include "direction.hpp"

class BlockComponentChangeDirection final : public BlockComponent
{
	public:
		BlockComponentChangeDirection( Direction::Simple direction );
		~BlockComponentChangeDirection();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const Direction::Simple direction_;
};