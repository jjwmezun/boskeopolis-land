#ifndef BLOCK_COMPONENT_WARP_H
#define BLOCK_COMPONENT_WARP_H

#include "block_component.hpp"

class BlockComponentWarp : public BlockComponent
{
	public:
		BlockComponentWarp();
		~BlockComponentWarp();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health ) override;
};

#endif // BLOCK_COMPONENT_WARP_H

