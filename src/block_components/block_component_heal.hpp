#pragma once

#include "block_component.hpp"

class BlockComponentHeal final : public BlockComponent
{
	public:
		BlockComponentHeal();
		~BlockComponentHeal();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
