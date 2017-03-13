#ifndef BOUNCY_CLOUD_SPRITE_H
#define BOUNCY_CLOUD_SPRITE_H

class Block;
class Input;

#include "sprite.h"

class BouncyCloudBlockSprite : public Sprite
{
	public:
		BouncyCloudBlockSprite( int x, int y );
		~BouncyCloudBlockSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
};

#endif // BOUNCY_CLOUD_SPRITE_H