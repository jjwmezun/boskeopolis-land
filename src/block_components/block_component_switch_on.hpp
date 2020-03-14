#pragma once

#include "block_component.hpp"

class BlockComponentSwitchOn final : public BlockComponent
{
	public:
		BlockComponentSwitchOn();
		~BlockComponentSwitchOn();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
