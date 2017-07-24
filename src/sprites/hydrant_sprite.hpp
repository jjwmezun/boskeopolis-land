



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
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"
    #include "timers/timer_simple.hpp"


// CLASS
//===================================

    class HydrantSprite : public Sprite
    {
        public:
            HydrantSprite( int x, int y );
            ~HydrantSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );

        private:
            TimerSimple delay_ = TimerSimple( 32, false );
            bool awake_ = false;
    };


#endif // HYDRANT_SPRITE_H
