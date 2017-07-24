#ifndef BLOCK_COMPONENT_LOSE_H
#define BLOCK_COMPONENT_LOSE_H

#include "block_component.hpp"

class BlockComponentLose : public BlockComponent
{
	public:
		BlockComponentLose();
		~BlockComponentLose();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health ) override;
};

#endif // BLOCK_COMPONENT_LOSE_H

