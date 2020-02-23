#pragma once

#include "sprite.hpp"

class SnowboulderSprite : public Sprite
{
	public:
		SnowboulderSprite( int x, int y );
		~SnowboulderSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};