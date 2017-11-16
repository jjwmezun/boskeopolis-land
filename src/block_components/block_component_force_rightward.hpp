#pragma once

#include "block_component.hpp"

class BlockComponentForceRightward : public BlockComponent
{
	public:
		BlockComponentForceRightward();
		~BlockComponentForceRightward();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};
