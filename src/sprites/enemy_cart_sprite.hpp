




// Name
//===================================
//
// EnemyCartSprite
//

#ifndef ENEMY_CART_SPRITE_H
#define ENEMY_CART_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class EnemyCartSprite : public Sprite
    {
        public:
            EnemyCartSprite( int x, int y );
            ~EnemyCartSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
    };


#endif // ENEMY_CART_SPRITE_H
