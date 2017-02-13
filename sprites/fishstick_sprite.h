




// Name
//===================================
//
// FishstickSprite
//

#ifndef FISHSTICK_SPRITE_H
#define FISHSTICK_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class FishstickSprite : public Sprite
    {
        public:
            FishstickSprite( int x, int y );
            ~FishstickSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
            void swim();

        private:
            TimerSimple move_timer_;
            TimerSimple pause_timer_;
    };


#endif // FISHSTICK_SPRITE_H
