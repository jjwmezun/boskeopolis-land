



// Name
//===================================
//
// IceCubeSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "icecube_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    IceCubeSprite::IceCubeSprite( int x, int y, Direction::Horizontal direction, bool start_moving )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVSPRITES_ICECUBE ) ), x, y, 32, 32, {}, 400, 1200, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, true, true, true ),
        start_moving_ ( start_moving ),
        start_moving_orig_ ( start_moving ),
        changed_ ( false )
    {};

    IceCubeSprite::~IceCubeSprite() {};

    void IceCubeSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( in_water_ )
        {
            changeMovement( SpriteMovement::Type::FLOATING );

            acceleration_y_ = 0;
            vy_ = 0;

            if ( start_moving_ )
            {
                if ( direction_x_ == Direction::Horizontal::LEFT )
                {
                    moveLeft();
                }
                else if ( direction_x_ == Direction::Horizontal::RIGHT )
                {
                    moveRight();
                }
            }

            if ( collide_right_ || collide_left_ )
            {
                acceleration_y_ = gravity_start_speed_;
            }

        }
    };

    void IceCubeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( their_collision.collideAny() )
            {
                them.collideStopAny( their_collision );
            }

            if ( their_collision.collideBottom() )
            {
                start_moving_ = true;
            }
        }
        else
        {
            if ( their_collision.collideAny() )
            {
                if ( !changed_ && start_moving_ )
                {
                    direction_x_ = Direction::switchHorizontal( direction_x_ );
                    changed_ = true;
                }
            }
        }
    };

    void IceCubeSprite::reset()
    {
        resetPosition();
        changeMovement( SpriteMovement::Type::GROUNDED );
        changed_ = false;
        start_moving_ = start_moving_orig_;
        fullStopX();
        fullStopY();
        direction_x_ = direction_x_orig_;
        in_water_ = false;
    };
