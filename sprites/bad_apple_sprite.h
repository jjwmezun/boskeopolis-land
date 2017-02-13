



// Name
//===================================
//
// BadAppleSprite
//

#ifndef BAD_APPLE_SPRITE_H
#define BAD_APPLE_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class BadAppleSprite : public Sprite
    {
        public:
            BadAppleSprite( int x, int y );
            ~BadAppleSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );
    };


#endif // BAD_APPLE_SPRITE_H
