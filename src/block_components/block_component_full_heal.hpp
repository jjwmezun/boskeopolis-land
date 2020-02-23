#pragma once

#include "block_component.hpp"

class BlockComponentFullHeal : public BlockComponent
{
	public:
		BlockComponentFullHeal();
		~BlockComponentFullHeal();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
