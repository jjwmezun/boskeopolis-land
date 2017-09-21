#pragma once

#include "block_component.hpp"

class BlockComponentLightSwitch : public BlockComponent
{
	public:
		BlockComponentLightSwitch();
		~BlockComponentLightSwitch();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health ) override;
};