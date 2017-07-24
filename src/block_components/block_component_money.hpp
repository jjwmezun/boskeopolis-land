#ifndef BLOCK_COMPONENT_MONEY_H
#define BLOCK_COMPONENT_MONEY_H

#include "block_component.hpp"

class BlockComponentMoney : public BlockComponent
{
	public:
		BlockComponentMoney( int amount = 100 );
		~BlockComponentMoney();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health ) override;

	private:
		int amount_;
};

#endif // BLOCK_COMPONENT_MONEY_H

