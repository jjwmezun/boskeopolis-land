#ifndef BLOCK_COMPONENT_SWIMMABLE_H
#define BLOCK_COMPONENT_SWIMMABLE_H

#include "block_component.hpp"

class BlockComponentSwimmable : public BlockComponent
{
	public:
		BlockComponentSwimmable();
		~BlockComponentSwimmable();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};

#endif // BLOCK_COMPONENT_SWIMMABLE_H

