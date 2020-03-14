#pragma once

#include "block_component.hpp"

class BlockComponentSink final : public BlockComponent
{
	public:
		BlockComponentSink();
		~BlockComponentSink();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
