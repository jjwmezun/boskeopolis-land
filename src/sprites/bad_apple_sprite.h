#ifndef BAD_APPLE_SPRITE_H
#define BAD_APPLE_SPRITE_H

class Block;
class Input;

#include "sprite.h"

class BadAppleSprite : public Sprite
{
	public:
		BadAppleSprite( int x, int y );
		~BadAppleSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
};


#endif // BAD_APPLE_SPRITE_H
