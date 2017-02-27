



// Name
//===================================
//
// EnemyCartSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "enemy_cart_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    EnemyCartSprite::EnemyCartSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVSPRITES_AUTUMN_CART ) ), x, y-16, 32, 32, { SpriteType::ENEMY }, 150, 3000, 3000 )
    {};

    EnemyCartSprite::~EnemyCartSprite() {};

    void EnemyCartSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        moveLeft();
    };

    void EnemyCartSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( their_collision.collideBottom() && !them.onGround() )
            {
                kill();
            }
            else if ( their_collision.collideAny() )
            {
                them.hurt();
            }
        }
    };
