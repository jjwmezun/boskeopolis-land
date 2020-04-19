#pragma once

#include "sprite.hpp"

class PolloFlameSprite final : public Sprite
{
	public:
		PolloFlameSprite( int x, int y, Direction::Horizontal direction );
		~PolloFlameSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};