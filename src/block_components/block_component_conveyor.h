#ifndef BLOCK_COMPONENT_CONVEYOR_H
#define BLOCK_COMPONENT_CONVEYOR_H

#include "block_component.h"
#include "direction.h"

class BlockComponentConveyor : public BlockComponent
{
	public:
		BlockComponentConveyor( Direction::Horizontal direction, int speed = 1000 );
		~BlockComponentConveyor();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;

	private:
		int x_effect_;
};

#endif // BLOCK_COMPONENT_CONVEYOR_H

