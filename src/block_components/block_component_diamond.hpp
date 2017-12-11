#ifndef BLOCK_COMPONENT_DIAMOND_H
#define BLOCK_COMPONENT_DIAMOND_H

#include "block_component.hpp"

class BlockComponentDiamond : public BlockComponent
{
	public:
		BlockComponentDiamond();
		~BlockComponentDiamond();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};

#endif // BLOCK_COMPONENT_DIAMOND_H

