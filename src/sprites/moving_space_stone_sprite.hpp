#pragma once

#include "sprite.hpp"

class MovingSpaceStoneSprite final : public Sprite
{
	public:
		MovingSpaceStoneSprite( int x, int y );
		~MovingSpaceStoneSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};
