


// Name
//===================================
//
// GuardSprite
//

#ifndef GUARD_SPRITE_H
#define GUARD_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class GuardSprite : public Sprite
    {
        public:
            GuardSprite( int x, int y );
            ~GuardSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );

        private:
            static const int DISTANCE_LIMIT_X = 8*16000 - 4000;
            static const int DISTANCE_LIMIT_Y = 16000;
            int distance_limit_y_ = DISTANCE_LIMIT_Y;
            TimerSimple stun_timer_ = TimerSimple( 8, false );
            bool stunned_ = false;
    };


#endif // GUARD_SPRITE_H
