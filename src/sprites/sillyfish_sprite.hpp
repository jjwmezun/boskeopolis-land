





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
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class SillyfishSprite : public Sprite
    {
        public:
            SillyfishSprite( int x, int y );
            ~SillyfishSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

        private:
            bool start_ground_lock_;
            TimerSimple switch_lock_;
    };


#endif // SILLYFISH_SPRITE_H
