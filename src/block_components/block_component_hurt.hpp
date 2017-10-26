#ifndef BLOCK_COMPONENT_HURT_H
#define BLOCK_COMPONENT_HURT_H

#include "block_component.hpp"

class BlockComponentHurt : public BlockComponent
{
	public:
		BlockComponentHurt();
		~BlockComponentHurt();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};

#endif // BLOCK_COMPONENT_HURT_H

