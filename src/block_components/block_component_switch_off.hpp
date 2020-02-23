#pragma once

#include "block_component.hpp"

class BlockComponentSwitchOff : public BlockComponent
{
	public:
		BlockComponentSwitchOff();
		~BlockComponentSwitchOff();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
