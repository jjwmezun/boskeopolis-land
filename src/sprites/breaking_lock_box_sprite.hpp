#pragma once

#include "sprite.hpp"

class BreakingLockBoxSprite final : public Sprite
{
	public:
		BreakingLockBoxSprite( int x, int y, int type );
		~BreakingLockBoxSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
