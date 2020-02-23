#pragma once

#include "sprite.hpp"

class QuadrapusSprite : public Sprite
{
	public:
		QuadrapusSprite( int x, int y );
		~QuadrapusSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};