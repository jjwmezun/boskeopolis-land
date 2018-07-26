#ifndef BLOCK_COMPONENT_CLIMBABLE_H
#define BLOCK_COMPONENT_CLIMBABLE_H

#include "block_component.hpp"

class BlockComponentClimbable : public BlockComponent
{
	public:
		BlockComponentClimbable();
		~BlockComponentClimbable();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const override;
};

#endif // BLOCK_COMPONENT_CLIMBABLE_H

