



// Name
//===================================
//
// HydrantSprite
//

#ifndef HYDRANT_SPRITE_H
#define HYDRANT_SPRITE_H


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

    class HydrantSprite : public Sprite
    {
        public:
            HydrantSprite( int x, int y );
            ~HydrantSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );

        private:
            TimerSimple delay_ = TimerSimple( 32, false );
            bool awake_ = false;
    };


#endif // HYDRANT_SPRITE_H
