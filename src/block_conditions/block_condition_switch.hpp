#pragma once

#include "block_condition.hpp"

class BlockConditionSwitch final : public BlockCondition
{
	public:
		BlockConditionSwitch( int target );
		~BlockConditionSwitch();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;

    private:
        int target_;
};