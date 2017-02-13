




// Name
//===================================
//
// FallingBoughSprite
//

#ifndef FALLING_BOUGH_SPRITE_H
#define FALLING_BOUGH_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"
    #include "timers/timer.h"


// CLASS
//===================================

    class FallingBoughSprite : public Sprite
    {
        public:
            FallingBoughSprite( int x, int y, Direction::Horizontal direction );
            ~FallingBoughSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
            void reset();

        private:
            TimerSimple falling_timer_ = TimerSimple( 8, false );
    };


#endif // FALLING_BOUGH_SPRITE_H
