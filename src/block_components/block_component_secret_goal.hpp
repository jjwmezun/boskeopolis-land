#pragma once

#include "block_component.hpp"

class BlockComponentSecretGoal final : public BlockComponent
{
	public:
		BlockComponentSecretGoal();
		~BlockComponentSecretGoal();
		void interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const override;
};
