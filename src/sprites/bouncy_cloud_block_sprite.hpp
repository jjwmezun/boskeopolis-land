#ifndef BOUNCY_CLOUD_SPRITE_H
#define BOUNCY_CLOUD_SPRITE_H

class Block;


#include "sprite.hpp"

class BouncyCloudBlockSprite : public Sprite
{
	public:
		BouncyCloudBlockSprite( int x, int y );
		~BouncyCloudBlockSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
};

#endif // BOUNCY_CLOUD_SPRITE_H