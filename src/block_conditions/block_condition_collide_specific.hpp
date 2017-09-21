#pragma once

#include "block_condition.hpp"

class BlockConditionCollideSpecific : public BlockCondition
{
	public:
		BlockConditionCollideSpecific( int left = 0, int right = 0, int top = 0, int bottom = 0 );
		~BlockConditionCollideSpecific();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;

	private:
		int left_;
		int right_;
		int top_;
		int bottom_;
};