#pragma once

#include "direction.hpp"
#include "sprite.hpp"

class FistMissileSprite final : public Sprite
{
	public:
		FistMissileSprite( int x, int y );
		~FistMissileSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
