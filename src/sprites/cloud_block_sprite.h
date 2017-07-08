



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
class Input;


// DEPENDENCIES
//===================================

#include "sprite.h"


// CLASS
//===================================

class CloudBlockSprite : public Sprite
{
	public:
		CloudBlockSprite( int x, int y );
		~CloudBlockSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
};

#endif // CLOUD_SPRITE_H