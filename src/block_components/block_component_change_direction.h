#ifndef BLOCK_COMPONENT_CHANGE_DIRECTION_H
#define BLOCK_COMPONENT_CHANGE_DIRECTION_H

#include "block_component.h"
#include "direction.h"

class BlockComponentChangeDirection : public BlockComponent
{
	public:
		BlockComponentChangeDirection( Direction::Simple direction );
		~BlockComponentChangeDirection();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, InventoryLevel& inventory, Camera& camera ) override;

	private:
		Direction::Simple direction_;
};

#endif // BLOCK_COMPONENT_CHANGE_DIRECTION_H

