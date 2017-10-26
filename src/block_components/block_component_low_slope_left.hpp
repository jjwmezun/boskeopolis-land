#ifndef BLOCK_COMPONENT_LOW_SLOPE_LEFT_H
#define BLOCK_COMPONENT_LOW_SLOPE_LEFT_H

#include "block_component.hpp"

class BlockComponentLowSlopeLeft : public BlockComponent
{
	public:
		BlockComponentLowSlopeLeft();
		~BlockComponentLowSlopeLeft();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};

#endif // BLOCK_COMPONENT_LOW_SLOPE_LEFT_H

