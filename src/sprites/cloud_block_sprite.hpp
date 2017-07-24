



// Name
//===================================
//
// CloudBlockSprite
//

#ifndef CLOUD_SPRITE_H
#define CLOUD_SPRITE_H


// FORWARD DECLARATIONS
//===================================

class Block;



// DEPENDENCIES
//===================================

#include "sprite.hpp"


// CLASS
//===================================

class CloudBlockSprite : public Sprite
{
	public:
		CloudBlockSprite( int x, int y );
		~CloudBlockSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
};

#endif // CLOUD_SPRITE_H
