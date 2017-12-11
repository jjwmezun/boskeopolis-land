#ifndef BLOCK_COMPONENT_CONVEYOR_H
#define BLOCK_COMPONENT_CONVEYOR_H

#include "block_component.hpp"
#include "direction.hpp"

class BlockComponentConveyor : public BlockComponent
{
	public:
		BlockComponentConveyor( Direction::Horizontal direction, int speed = 1000 );
		~BlockComponentConveyor();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) override;

	private:
		int x_effect_;
};

#endif // BLOCK_COMPONENT_CONVEYOR_H

