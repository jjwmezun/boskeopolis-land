#ifndef BLOCK_COMPONENT_SOLID_H
#define BLOCK_COMPONENT_SOLID_H

#include "block_component.h"

class BlockComponentSolid : public BlockComponent
{
	public:
		BlockComponentSolid();
		~BlockComponentSolid();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_SOLID_H

