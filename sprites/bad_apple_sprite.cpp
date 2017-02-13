





// Name
//===================================
//
// BadAppleSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "bad_apple_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BadAppleSprite::BadAppleSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVSPRITES_BADAPPLE ) ), x, y, 16, 16, SpriteType::ENEMY, 100, 600, 1000, 1000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
    {
        direction_x_ = Direction::Horizontal::LEFT;
    };

    BadAppleSprite::~BadAppleSprite() {};

    void BadAppleSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
        if ( isDead() )
            block_interact_ = false;

        if ( direction_x_ == Direction::Horizontal::RIGHT )
        {
            moveRight();
        }
        else
        {
            moveLeft();
        }

        if ( collidedRight() )
        {
            direction_x_ = Direction::Horizontal::LEFT;
        }
        if ( collidedLeft() )
        {
            direction_x_ = Direction::Horizontal::RIGHT;
        }
    };

    void BadAppleSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( them.collideBottom( their_collision, this ) )
            {
                kill();
                them.bounce();
            }
            else if ( their_collision.collideAny() && !isDead() )
            {
                them.hurt();
            }
        }
    };
