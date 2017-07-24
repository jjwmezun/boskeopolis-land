#pragma once

#include "block_component.hpp"

class BlockComponentSteepSlopeRight : public BlockComponent
{
	public:
		BlockComponentSteepSlopeRight();
		~BlockComponentSteepSlopeRight();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health ) override;
};