#pragma once

#include "block_condition.hpp"

class BlockConditionCollideSpecific : public BlockCondition
{
	public:
		BlockConditionCollideSpecific( int left = 0, int right = 0, int top = 0, int bottom = 0 );
		~BlockConditionCollideSpecific();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, LevelState& level_state ) const override;

	private:
		const int left_;
		const int right_;
		const int top_;
		const int bottom_;
};
