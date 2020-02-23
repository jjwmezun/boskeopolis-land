#pragma once

#include "block_component.hpp"

class BlockComponentSwitchSound : public BlockComponent
{
	public:
		BlockComponentSwitchSound();
		~BlockComponentSwitchSound();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};

