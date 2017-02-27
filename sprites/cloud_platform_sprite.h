


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
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"
    #include "timers/timer_simple.h"


// CLASS
//===================================

    class CloudPlatformSprite : public Sprite
    {
        public:
            CloudPlatformSprite( int x, int y, Direction::Horizontal direction );
            ~CloudPlatformSprite();
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

        private:
            bool started_;
			int map_width_;
    };


#endif // CLOUD_PLATFORM_SPRITE_H
