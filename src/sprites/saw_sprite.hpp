




// Name
//===================================
//
// SawSprite
//

#ifndef SAW_SPRITE_H
#define SAW_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"
    #include "timers/timer_simple.hpp"


// CLASS
//===================================

    class SawSprite : public Sprite
    {
        public:
            SawSprite( int x, int y );
            ~SawSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );

        private:
            int counter_ = 0;
            bool going_up_ = false;
    };


#endif // SAW_SPRITE_H
