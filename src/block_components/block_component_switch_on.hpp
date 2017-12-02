#pragma once

#include "block_component.hpp"

class BlockComponentSwitchOn : public BlockComponent
{
	public:
		BlockComponentSwitchOn();
		~BlockComponentSwitchOn();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};