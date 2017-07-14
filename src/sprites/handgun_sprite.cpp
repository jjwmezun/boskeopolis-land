







// Name
//===================================
//
// HandgunSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "handgun_sprite.h"
    #include "mezun_math.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    HandgunSprite::HandgunSprite( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/handgun.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x+2, y+2, 28, 28, {}, 200, 5000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_INSTANTLY_OFFSCREEN, false, false )
    {};

    HandgunSprite::~HandgunSprite() {};

    void HandgunSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( shooting_ )
        {
            moveLeft();
        }
        else
        {
            if ( can_shoot_ )
            {
                testIfItShouldShoot();
            }
        }

        can_shoot_ = true;
    };

    void HandgunSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( shooting_ )
            {
                if ( their_collision.collideBottom() )
                {
                    them.bounce();
                    kill();
                    changeMovement( SpriteMovement::Type::GROUNDED );
                }
                else if ( their_collision.collideAny() )
                {
                    them.hurt();
                }
            }
            else if
            (
                them.rightSubPixels()  <= leftSubPixels()   + RANGE_INTERVAL &&
                them.rightSubPixels()  >= leftSubPixels()   - RANGE_INTERVAL &&
                them.topSubPixels()    >= topSubPixels()    - RANGE_INTERVAL &&
                them.bottomSubPixels() <= bottomSubPixels() + RANGE_INTERVAL
            )
            {
                can_shoot_ = false;
            }
        }
    };

    void HandgunSprite::testIfItShouldShoot()
    {
        shooting_ = mezun::testRandomWithinPercentage( SHOOT_CHANCE_PERCENT );
    };

    void HandgunSprite::reset()
    {
        resetPosition();
        shooting_ = false;
        acceleration_x_ = 0;
        vx_ = 0;
        acceleration_y_ = 0;
        vy_ = 0;
        changeMovement( SpriteMovement::Type::FLOATING );
        status_.reset();
    };
