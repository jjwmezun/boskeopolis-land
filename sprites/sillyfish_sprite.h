





// Name
//===================================
//
// SillyfishSprite
//

#ifndef SILLYFISH_SPRITE_H
#define SILLYFISH_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class SillyfishSprite : public Sprite
    {
        public:
            SillyfishSprite( int x, int y );
            ~SillyfishSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );

        private:
            bool start_ground_lock_;
            TimerSimple switch_lock_;
    };


#endif // SILLYFISH_SPRITE_H
