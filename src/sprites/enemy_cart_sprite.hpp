




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

    class EnemyCartSprite final : public Sprite
    {
        public:
            EnemyCartSprite( int x, int y );
            ~EnemyCartSprite();
            void customUpdate( LevelState& level_state );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
    };


#endif // ENEMY_CART_SPRITE_H
