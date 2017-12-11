#ifndef BLOCK_COMPONENT_SOLID_H
#define BLOCK_COMPONENT_SOLID_H

#include "block_component.hpp"

class BlockComponentSolid : public BlockComponent
{
	public:
		BlockComponentSolid();
		~BlockComponentSolid();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};

#endif // BLOCK_COMPONENT_SOLID_H

