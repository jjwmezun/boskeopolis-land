#pragma once

#include "sprite.h"

class SnowboulderSprite : public Sprite
{
	public:
		SnowboulderSprite( int x, int y );
		~SnowboulderSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
};