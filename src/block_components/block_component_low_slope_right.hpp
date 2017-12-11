#ifndef BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H
#define BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H

#include "block_component.hpp"

class BlockComponentLowSlopeRight : public BlockComponent
{
	public:
		BlockComponentLowSlopeRight();
		~BlockComponentLowSlopeRight();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};

#endif // BLOCK_COMPONENT_LOW_SLOPE_RIGHT_H

