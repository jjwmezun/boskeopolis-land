#ifndef BLOCK_COMPONENT_CHANGE_DIRECTION_H
#define BLOCK_COMPONENT_CHANGE_DIRECTION_H

#include "block_component.hpp"
#include "direction.hpp"

class BlockComponentChangeDirection : public BlockComponent
{
	public:
		BlockComponentChangeDirection( Direction::Simple direction );
		~BlockComponentChangeDirection();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;

	private:
		const Direction::Simple direction_;
};

#endif // BLOCK_COMPONENT_CHANGE_DIRECTION_H
