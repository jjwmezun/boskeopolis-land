




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
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"
    #include "timers/timer_simple.h"


// CLASS
//===================================

    class SawSprite : public Sprite
    {
        public:
            SawSprite( int x, int y );
            ~SawSprite();
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

        private:
            int counter_ = 0;
            bool going_up_ = false;
    };


#endif // SAW_SPRITE_H
