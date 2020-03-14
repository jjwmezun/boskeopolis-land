



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

class CloudBlockSprite final : public Sprite
{
	public:
		CloudBlockSprite( int x, int y );
		~CloudBlockSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
};

#endif // CLOUD_SPRITE_H
