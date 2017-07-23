



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
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

        private:
            TimerSimple delay_ = TimerSimple( 32, false );
            bool awake_ = false;
    };


#endif // HYDRANT_SPRITE_H
