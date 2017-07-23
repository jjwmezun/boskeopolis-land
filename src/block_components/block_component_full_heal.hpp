#ifndef BLOCK_COMPONENT_FULL_HEAL_H
#define BLOCK_COMPONENT_FULL_HEAL_H

#include "block_component.hpp"

class BlockComponentFullHeal : public BlockComponent
{
	public:
		BlockComponentFullHeal();
		~BlockComponentFullHeal();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera ) override;
};

#endif // BLOCK_COMPONENT_FULL_HEAL_H

