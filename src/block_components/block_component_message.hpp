#ifndef BLOCK_COMPONENT_MESSAGE_H
#define BLOCK_COMPONENT_MESSAGE_H

#include "block_component.hpp"

class BlockComponentMessage : public BlockComponent
{
	public:
		BlockComponentMessage();
		~BlockComponentMessage();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};

#endif // BLOCK_COMPONENT_MESSAGE_H

