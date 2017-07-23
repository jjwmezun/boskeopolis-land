#pragma once

#include "block_component.hpp"

class BlockComponentDoor : public BlockComponent
{
	public:
		BlockComponentDoor();
		~BlockComponentDoor();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera ) override;
};

