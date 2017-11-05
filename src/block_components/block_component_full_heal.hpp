#ifndef BLOCK_COMPONENT_FULL_HEAL_H
#define BLOCK_COMPONENT_FULL_HEAL_H

#include "block_component.hpp"

class BlockComponentFullHeal : public BlockComponent
{
	public:
		BlockComponentFullHeal();
		~BlockComponentFullHeal();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks ) override;
};

#endif // BLOCK_COMPONENT_FULL_HEAL_H

