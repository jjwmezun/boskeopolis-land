


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
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class BuzzSawSprite : public Sprite
    {
        public:
            BuzzSawSprite( int x, int y );
            ~BuzzSawSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
    };


#endif // BUZZ_SAW_SPRITE_H
