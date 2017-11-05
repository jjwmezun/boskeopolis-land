#ifndef BLOCK_COMPONENT_GOAL_H
#define BLOCK_COMPONENT_GOAL_H

#include "block_component.hpp"

class BlockComponentGoal : public BlockComponent
{
	public:
		BlockComponentGoal();
		~BlockComponentGoal();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};

#endif // BLOCK_COMPONENT_GOAL_H

