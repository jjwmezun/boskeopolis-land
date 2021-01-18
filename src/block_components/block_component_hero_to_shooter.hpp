#pragma once

#include "block_component.hpp"

class BlockComponentHeroToShooter final : public BlockComponent
{
	public:
		BlockComponentHeroToShooter();
		~BlockComponentHeroToShooter();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
