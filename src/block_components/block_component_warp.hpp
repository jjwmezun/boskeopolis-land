#ifndef BLOCK_COMPONENT_WARP_H
#define BLOCK_COMPONENT_WARP_H

#include "block_component.hpp"

class BlockComponentWarp : public BlockComponent
{
	public:
		BlockComponentWarp();
		~BlockComponentWarp();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;
};

#endif // BLOCK_COMPONENT_WARP_H

