#pragma once

#include "block_component.hpp"

class BlockComponentDoor : public BlockComponent
{
	public:
		BlockComponentDoor();
		~BlockComponentDoor();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};

