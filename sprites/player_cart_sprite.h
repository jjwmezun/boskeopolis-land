

// Name
//===================================
//
// PlayerCartSprite
//

#ifndef PLAYER_CART_SPRITE_H
#define PLAYER_CART_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;
    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class PlayerCartSprite : public Sprite
    {
        public:
            PlayerCartSprite( int x, int y );
            ~PlayerCartSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );

        private:
            static const int JUMP_LIMIT = Unit::BlocksToSubPixels( 32 );
            bool reached_height_ = false;
            void duck();
            void unduck();
    };


#endif // PLAYER_CART_SPRITE_H
