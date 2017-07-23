#ifndef BLOCK_COMPONENT_CHANGE_BLOCK_H
#define BLOCK_COMPONENT_CHANGE_BLOCK_H

#include "block_component.hpp"

class BlockComponentChangeBlock : public BlockComponent
{
	public:
		BlockComponentChangeBlock( int new_block );
		~BlockComponentChangeBlock();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& eventsLevel, Camera& camera ) override;

	private:
		int new_block_;
};

#endif // BLOCK_COMPONENT_CHANGE_BLOCK_H

