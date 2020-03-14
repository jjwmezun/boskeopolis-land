#pragma once

#include "block_component.hpp"

class BlockComponentShmupBullet final : public BlockComponent
{
	public:
		BlockComponentShmupBullet();
		~BlockComponentShmupBullet();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
