




// Name
//===================================
//
// PlayerCartSprite
//


// DEPENDENCIES
//===================================

    #include "camera.h"
    #include "event_system.h"
    #include "graphics.h"
    #include "input.h"
    #include "map.h"
    #include "player_cart_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PlayerCartSprite::PlayerCartSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVSPRITES_AUTUMN_CART ) ), x, y, 32, 40, { SpriteType::HERO }, 160, 5000, 1000, 7000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, false, true, true, false, .8 )
    {
        run();
        direction_x_ = Direction::Horizontal::RIGHT;
    };

    PlayerCartSprite::~PlayerCartSprite() {};

    void PlayerCartSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
        if ( direction_x_ == Direction::Horizontal::RIGHT )
        {
            if ( collide_right_ )
            {
                status_.hurt();
            }

            if ( acceleration_x_ < 0 )
            {
                acceleration_x_ = 0;
            }

            if ( vx_ < 0 )
            {
                vx_ = 0;
            }

            moveRight();
        }
        else if ( direction_x_ == Direction::Horizontal::LEFT )
        {
            if ( collide_left_ )
            {
                status_.hurt();
            }

            if ( acceleration_x_ > 0 )
            {
                acceleration_x_ = 0;
            }

            if ( vx_ > 0 )
            {
                vx_ = 0;
            }

            moveLeft();
        }
        else
        {
            if ( !reached_height_ )
            {
                acceleration_x_ = 0;
                vx_ = 0;
                changeMovement( SpriteMovement::Type::FLOATING );
                acceleration_y_ = -jump_start_speed_;

                if ( topSubPixels() <= JUMP_LIMIT )
                {
                    reached_height_ = true;
                }
            }
            else
            {
                changeMovement( SpriteMovement::Type::GROUNDED );

                if ( on_ground_prev_ )
                {
                    direction_x_ = Direction::Horizontal::RIGHT;
                }
            }
        }

        if ( input.held( Input::Action::JUMP ) )
        {
            jump();
            jump_lock_ = true;
        }

        if ( input.released( Input::Action::JUMP ) )
        {
            if ( !isDucking() )
            {
                jump_lock_ = false;
            }
        }

        if ( input.held( Input::Action::MOVE_DOWN ) )
        {
            duck();
        }

        if ( input.released( Input::Action::MOVE_DOWN ) )
        {
            unduck();
        }

        if ( isDucking() )
        {
            jump_lock_ = true;
        }

        if (  fellInBottomlessPit( lvmap ) )
        {
            status_.kill();
        }

        camera.adjustCart( *this, lvmap );

        if ( xPixels() > lvmap.widthPixels() + widthPixels() )
        {
            events.won_ = true;
        }
    };

    void PlayerCartSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
    };

    void PlayerCartSprite::duck()
    {
        // Hacky way to make player warp to the right position after height changes.
        if ( !isDucking() )
        {
            hit_box_.y += Unit::PixelsToSubPixels( 8 );
            //graphics_->y_adjustment_ = -5;
            //graphics_->h_adjustment_ = -16;
        }

        is_ducking_ = true;

        hit_box_.h = original_hit_box_.h - Unit::PixelsToSubPixels( 8 );
    };

    void PlayerCartSprite::unduck()
    {
        // Hacky way to keep player from falling through ground after gaining height from unducking.
        if ( isDucking() )
        {
            hit_box_.y -= Unit::PixelsToSubPixels( 8 );
            //graphics_->y_adjustment_ = -1;
            //graphics_->h_adjustment_ = 2;
        }

        is_ducking_ = false;
        jump_lock_ = false;
        hit_box_.h = original_hit_box_.h;
    };
