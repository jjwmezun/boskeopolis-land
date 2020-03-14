#pragma once

#include "sprite.hpp"

class ShmupShooterSprite final : public Sprite
{
	public:
		ShmupShooterSprite( int x, int y );
		~ShmupShooterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
