#ifndef BLOCK_COMPONENT_KILL_H
#define BLOCK_COMPONENT_KILL_H

#include "block_component.h"

class BlockComponentKill : public BlockComponent
{
	public:
		BlockComponentKill();
		~BlockComponentKill();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_KILL_H

