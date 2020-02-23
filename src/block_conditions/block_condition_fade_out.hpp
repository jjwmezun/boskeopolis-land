#pragma once

#include "block_condition.hpp"

class BlockConditionFadeOut : public BlockCondition
{
	public:
		BlockConditionFadeOut( int threshold = 0 );
		~BlockConditionFadeOut();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const LevelState& level_state ) const override;
		
	private:
		const int threshold_;
};