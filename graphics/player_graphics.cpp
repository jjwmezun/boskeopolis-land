


// Name
//===================================
//
// PlayerGraphics
//


// DEPENDENCIES
//===================================

    #include "player_graphics.h"
    #include "sprite.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PlayerGraphics::PlayerGraphics( Graphics::SpriteSheet texture )
    :
        SpriteGraphics ( texture, 0, 0, false, false, 0, false, -1, -2, 2, 4 )
    {};

    PlayerGraphics::~PlayerGraphics() {};

    void PlayerGraphics::update( Sprite& sprite )
    {
        if ( sprite.directionX() == Direction::Horizontal::RIGHT )
        {
            flip_x_ = true;
        }
        else
        {
            flip_x_ = false;
        }

        if ( sprite.isDead() )
        {
            current_frame_x_ = 16;
            current_frame_y_ = 26;
        }
        else if ( sprite.onLadder() )
        {
            if ( sprite.isMoving() )
            {
                if ( animation_timer_.hit() )
                {
                    ++climb_counter_;
                }

                animation_timer_.update();
            }

            switch ( climb_counter_.value() )
            {
                case 1:
                    flip_x_ = !flip_x_;
                    break;
            }

            current_frame_x_ = 96;
            current_frame_y_ = 0;
        }
        else if ( sprite.isSlidingPrev() )
        {
            current_frame_x_ = 48;
            current_frame_y_ = 26;
        }
        else if ( sprite.isDucking() )
        {
            if ( animation_timer_.hit() )
            {
                ++blink_counter_;
            }

            animation_timer_.update();

            switch ( blink_counter_.value() )
            {
                case 3:
                    current_frame_x_ = 0;
                    current_frame_y_ = 32;
                    break;
                default:
                    current_frame_x_ = 64;
                    current_frame_y_ = 6;
            }
        }
        else if ( !sprite.onGroundPrev() )
        {
            current_frame_x_ = 48;
            current_frame_y_ = 0;
        }
        else if ( sprite.isMoving() )
        {
            if ( animation_timer_.hit() )
            {
                ++walk_counter_;
            }

            animation_timer_.update();

            switch ( walk_counter_.value() )
            {
                case 1:
                    current_frame_x_ = 0;
                    break;
                case 2:
                    current_frame_x_ = 32;
                    break;
                case 3:
                    current_frame_x_ = 0;
                    break;
                default:
                    current_frame_x_ = 16;
            }

            current_frame_y_ = 0;
        }
        else
        {
            if ( animation_timer_.hit() )
            {
                ++blink_counter_;
            }

            animation_timer_.update();

            switch ( blink_counter_.value() )
            {
                case 3:
                    current_frame_x_ = 112;
                    break;
                default:
                    if ( sprite.lookingUp() )
                        current_frame_x_ = 80;
                    else
                        current_frame_x_ = 0;
            }

            current_frame_y_ = 0;
        }
    };

