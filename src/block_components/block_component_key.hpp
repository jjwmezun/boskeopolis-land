#ifndef BLOCK_COMPONENT_KEY_H
#define BLOCK_COMPONENT_KEY_H

#include "block_component.hpp"

class BlockComponentKey : public BlockComponent
{
	public:
		BlockComponentKey();
		~BlockComponentKey();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};

#endif // BLOCK_COMPONENT_KEY_H

