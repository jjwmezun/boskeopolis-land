#pragma once

#include "block_condition.hpp"

class BlockConditionFadeOut : public BlockCondition
{
	public:
		BlockConditionFadeOut( int threshold = 0 );
		~BlockConditionFadeOut();
		bool condition( Collision& collision, Sprite& sprite, Block& block, EventSystem& events ) override;
		
	private:
		const int threshold_;
};