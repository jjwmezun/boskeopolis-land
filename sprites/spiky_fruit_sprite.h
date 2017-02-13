




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
    class Input;


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
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
            void reset();
    };


#endif // SPIKY_FRUIT_SPRITE_H
