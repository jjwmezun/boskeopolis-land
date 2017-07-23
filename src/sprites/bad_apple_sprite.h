#ifndef BAD_APPLE_SPRITE_H
#define BAD_APPLE_SPRITE_H

class Block;


#include "sprite.h"

class BadAppleSprite : public Sprite
{
	public:
		BadAppleSprite( int x, int y );
		~BadAppleSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
};


#endif // BAD_APPLE_SPRITE_H
