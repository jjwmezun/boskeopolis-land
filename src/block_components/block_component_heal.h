#ifndef BLOCK_COMPONENT_HEAL_H
#define BLOCK_COMPONENT_HEAL_H

#include "block_component.h"

class BlockComponentHeal : public BlockComponent
{
	public:
		BlockComponentHeal( int amount = 1 );
		~BlockComponentHeal();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;

	private:
		int amount_;
};

#endif // BLOCK_COMPONENT_HEAL_H
