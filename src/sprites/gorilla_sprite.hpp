#pragma once

#include "sprite.hpp"

class GorillaSprite final : public Sprite
{
	public:
		GorillaSprite( int x, int y );
		~GorillaSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
