


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
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class GuardSprite : public Sprite
    {
        public:
            GuardSprite( int x, int y );
            ~GuardSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );

        private:
            static const int DISTANCE_LIMIT_X = 8*16000 - 4000;
            static const int DISTANCE_LIMIT_Y = 16000;
            int distance_limit_y_ = DISTANCE_LIMIT_Y;
            TimerSimple stun_timer_ = TimerSimple( 8, false );
            bool stunned_ = false;
    };


#endif // GUARD_SPRITE_H
