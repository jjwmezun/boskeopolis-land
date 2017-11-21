#pragma once

#include "block_condition.hpp"

class BlockConditionCollideSpecific : public BlockCondition
{
	public:
		BlockConditionCollideSpecific( int left = 0, int right = 0, int top = 0, int bottom = 0 );
		~BlockConditionCollideSpecific();
		bool condition( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health ) override;

	private:
		int left_;
		int right_;
		int top_;
		int bottom_;
};