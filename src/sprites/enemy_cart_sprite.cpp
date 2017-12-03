



// Name
//===================================
//
// EnemyCartSprite
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "enemy_cart_sprite.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    EnemyCartSprite::EnemyCartSprite( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/autumn_cart.png", 144, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 44, 44, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 150, 3000, 3000 )
    {};

    EnemyCartSprite::~EnemyCartSprite() {};

    void EnemyCartSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
    {
        moveLeft();
    };

    void EnemyCartSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
    {
    };
