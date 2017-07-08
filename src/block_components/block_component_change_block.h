#ifndef BLOCK_COMPONENT_CHANGE_BLOCK_H
#define BLOCK_COMPONENT_CHANGE_BLOCK_H

#include "block_component.h"

class BlockComponentChangeBlock : public BlockComponent
{
	public:
		BlockComponentChangeBlock( int new_block );
		~BlockComponentChangeBlock();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& InventoryLevel, Camera& camera ) override;

	private:
		int new_block_;
};

#endif // BLOCK_COMPONENT_CHANGE_BLOCK_H

