#pragma once

#include "sprite.hpp"

class LightningSprite : public Sprite
{
	public:
		LightningSprite( int x, int y );
		~LightningSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
};