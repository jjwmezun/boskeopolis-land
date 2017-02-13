





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
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"
    #include "timers/timer_simple.h"


// CLASS
//===================================

    class PlatformSprite : public Sprite
    {
        public:
            PlatformSprite( int x, int y, Direction::Simple direction, int move_space_before_change = 128000 );
            ~PlatformSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
    };


#endif // PLATFORM_SPRITE_H
