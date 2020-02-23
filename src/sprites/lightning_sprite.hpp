#pragma once

#include "sprite.hpp"

class LightningSprite : public Sprite
{
	public:
		LightningSprite( int x, int y );
		~LightningSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};