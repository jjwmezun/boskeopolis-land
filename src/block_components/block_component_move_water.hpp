#pragma once

#include "block_component.hpp"

class BlockComponentMoveWater : public BlockComponent
{
	public:
		BlockComponentMoveWater();
		~BlockComponentMoveWater();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health ) override;
};