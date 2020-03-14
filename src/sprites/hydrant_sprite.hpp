



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

    class HydrantSprite final : public Sprite
    {
        public:
            HydrantSprite( int x, int y );
            ~HydrantSprite();
            void customUpdate( LevelState& level_state );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

        private:
            TimerSimple delay_ = TimerSimple( 32, false );
            bool awake_ = false;
    };


#endif // HYDRANT_SPRITE_H
