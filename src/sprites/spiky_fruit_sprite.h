




// Name
//===================================
//
// SpikyFruitSprite
//

#ifndef SPIKY_FRUIT_SPRITE_H
#define SPIKY_FRUIT_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class SpikyFruitSprite : public Sprite
    {
        public:
            SpikyFruitSprite( int x, int y );
            ~SpikyFruitSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
            void reset();
    };


#endif // SPIKY_FRUIT_SPRITE_H
