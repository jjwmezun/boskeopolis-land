


// Name
//===================================
//
// BuzzSawSprite
//

#ifndef BUZZ_SAW_SPRITE_H
#define BUZZ_SAW_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class BuzzSawSprite : public Sprite
    {
        public:
            BuzzSawSprite( int x, int y );
            ~BuzzSawSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
    };


#endif // BUZZ_SAW_SPRITE_H
