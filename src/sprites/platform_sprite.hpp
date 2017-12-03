





// Name
//===================================
//
// PlatformSprite
//

#ifndef PLATFORM_SPRITE_H
#define PLATFORM_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"
    #include "timers/timer_simple.hpp"


// CLASS
//===================================

    class PlatformSprite : public Sprite
    {
        public:
            PlatformSprite( int x, int y, Direction::Simple direction, int move_space_before_change = 128000 );
            ~PlatformSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );
    };


#endif // PLATFORM_SPRITE_H
