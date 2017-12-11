#ifndef BLOCK_COMPONENT_HIGH_SLOPE_LEFT_H
#define BLOCK_COMPONENT_HIGH_SLOPE_LEFT_H

#include "block_component.hpp"

class BlockComponentHighSlopeLeft : public BlockComponent
{
	public:
		BlockComponentHighSlopeLeft();
		~BlockComponentHighSlopeLeft();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};

#endif // BLOCK_COMPONENT_HIGH_SLOPE_LEFT_H

