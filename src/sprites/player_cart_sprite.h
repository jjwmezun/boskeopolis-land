

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
            PlayerCartSprite( int x, int y, int max_hp, int hp );
            ~PlayerCartSprite();
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

        private:
            static const int JUMP_LIMIT = Unit::BlocksToSubPixels( 32 );
            bool reached_height_ = false;
            void duck();
            void unduck();
    };


#endif // PLAYER_CART_SPRITE_H
