#pragma once

class Block;

#include "sprite.hpp"

class RotatingBlockSprite final : public Sprite
{
	public:
		RotatingBlockSprite( int x, int y );
		~RotatingBlockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};