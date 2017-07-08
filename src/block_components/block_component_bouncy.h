#ifndef BLOCK_COMPONENT_BOUNCY_H
#define BLOCK_COMPONENT_BOUNCY_H

#include "block_component.h"

class BlockComponentBouncy : public BlockComponent
{
	public:
		BlockComponentBouncy( int strength = 24 );
		~BlockComponentBouncy();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;

	private:
		const int strength_;
};

#endif // BLOCK_COMPONENT_BOUNCY_H

