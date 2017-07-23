


// Name
//===================================
//
// CloudPlatformSprite
//

#ifndef CLOUD_PLATFORM_SPRITE_H
#define CLOUD_PLATFORM_SPRITE_H


// FORWARD DECLARATIONS
//===================================

class Block;



// DEPENDENCIES
//===================================

#include "sprite.hpp"
#include "timers/timer_simple.hpp"


// CLASS
//===================================

class CloudPlatformSprite : public Sprite
{
	public:
		CloudPlatformSprite( int x, int y, Direction::Horizontal direction );
		~CloudPlatformSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

	private:
		bool started_;
};


#endif // CLOUD_PLATFORM_SPRITE_H
