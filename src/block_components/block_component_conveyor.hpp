#ifndef BLOCK_COMPONENT_CONVEYOR_H
#define BLOCK_COMPONENT_CONVEYOR_H

#include "block_component.hpp"
#include "direction.hpp"

class BlockComponentConveyor final : public BlockComponent
{
	public:
		BlockComponentConveyor( Direction::Horizontal direction, int speed = 1000 );
		~BlockComponentConveyor();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const int x_effect_;
};

#endif // BLOCK_COMPONENT_CONVEYOR_H
