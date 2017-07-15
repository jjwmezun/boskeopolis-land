



// Name
//===================================
//
// GroundedSpriteMovement
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "sprite.h"
    #include "grounded_sprite_movement.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    GroundedSpriteMovement::GroundedSpriteMovement()
    :
        SpriteMovement( Type::GROUNDED )
    {};

    GroundedSpriteMovement::GroundedSpriteMovement( Type type )
    :
        SpriteMovement( type )
    {};

    void GroundedSpriteMovement::moveUp( Sprite& sprite )
    {
        if ( sprite.onLadder() )
        {
            if ( sprite.isJumping() )
			{
                sprite.acceleration_y_ = -sprite.LADDER_SPEED * 2;
			}
            else
			{
                sprite.acceleration_y_ = -sprite.LADDER_SPEED;
			}
        }
        else if ( sprite.onGround() )
        {
            sprite.lookUp();
        }
    };

    void GroundedSpriteMovement::moveDown( Sprite& sprite )
    {
        if ( sprite.onLadder() )
        {
            sprite.acceleration_y_ = sprite.LADDER_SPEED;
        }
    };

    void GroundedSpriteMovement::jump( Sprite& sprite )
    {
        sprite.top_speed_upward_ = sprite.jump_top_speed_;

        if ( !sprite.onGround() || !sprite.jump_lock_ )
        {
            if ( ( onGroundPadding( sprite ) || sprite.onLadder() ) && !sprite.jump_lock_ )
            {
                startJump( sprite );
            }

            if ( sprite.jump_start_ && !sprite.jump_end_ )
            {
                sprite.is_jumping_ = true;
            }

            if ( sprite.isJumping() )
            {
                sprite.acceleration_y_ = -sprite.jump_start_speed_;
            }
        }

        if ( sprite.vy_ <= -sprite.jump_top_speed_ )
        {
            sprite.jump_start_ = false;
        }
    };

    void GroundedSpriteMovement::bounce( Sprite& sprite, int amount )
    {
        if ( bounce_height_ == 0 )
            bounce_height_ = Unit::PixelsToSubPixels( amount );

        sprite.is_bouncing_ = true;
        sprite.acceleration_y_ = -sprite.jump_start_speed_ * 8;

        sprite.top_speed_upward_ = bounce_height_;

        if ( sprite.vy_ <= -bounce_height_ )
        {
            sprite.is_bouncing_ = false;
            bounce_height_ = 0;
        }
    };

    void GroundedSpriteMovement::position( Sprite& sprite )
    {/*
        if ( sprite.slide_jump_ )
        {
            sprite.vx_ *= 5;
        }*/

        if ( !sprite.onGround() )
        {
            if ( sprite.isRunning() )
            {
                sprite.top_speed_ = sprite.top_speed_run_ * .75;
            }
            else
            {
                sprite.top_speed_ = sprite.top_speed_walk_ * .75;
            }
        }

        if ( sprite.isDucking() && !sprite.slide_jump_ )
        {
            sprite.acceleration_x_ /= 1.5;
        }

        if ( !sprite.onGround() && !sprite.isJumping() && !sprite.onLadder() && !sprite.is_bouncing_ )
        {
            sprite.acceleration_y_ = sprite.gravity_start_speed_;
        }

        sprite.top_speed_downward_ = sprite.gravity_top_speed_;

        if ( !sprite.isJumping() )
        {
            sprite.jump_end_ = true;
        }

        if ( sprite.onGround() )
        {
            sprite.slide_jump_ = false;
        }

        if ( sprite.on_ground_padding_.on() )
        {
            sprite.on_ground_padding_.update();
        }

        if ( sprite.on_ground_prev_ && !sprite.on_ground_ && !sprite.isJumping() )
        {
            sprite.on_ground_padding_.stop();
            sprite.on_ground_padding_.start();
        }
        else if ( sprite.on_ground_ || sprite.isJumping() )
        {
            sprite.on_ground_padding_.stop();
        }

        sprite.is_jumping_ = false;
        sprite.on_ground_prev_ = sprite.on_ground_;
        sprite.on_ground_ = false;
        sprite.touching_ladder_prev_ = sprite.touching_ladder_;
        sprite.touching_ladder_ = false;
        sprite.is_bouncing_prev_ = sprite.is_bouncing_;

        if ( sprite.onLadder() )
        {
            sprite.vy_ = 0;
        }

        // If not moving anymo', start slowing down.
        if ( sprite.acceleration_x_ == 0 )
            sprite.vx_ /= sprite.traction_;
    };

    void GroundedSpriteMovement::startJump( Sprite& sprite )
    {
        sprite.jump_start_ = true;
        sprite.jump_end_ = false;
        sprite.vy_ = 0;
        sprite.on_ground_ = false;
        sprite.on_ladder_ = false;

        if ( sprite.is_sliding_ )
        {
            sprite.slide_jump_ = true;
			sprite.vx_ *= 5;
        }
    };

    void GroundedSpriteMovement::collideStopYBottom( Sprite& sprite, int overlap )
    {
        if ( sprite.vy_ > 0 )
        {
            sprite.acceleration_y_ = 0;

            if ( !sprite.onGround() )
            {
                sprite.hit_box_.y -= overlap;
            }

            sprite.on_ground_ = true;
        }

        if ( sprite.onLadder() )
        {
            sprite.hit_box_.y -= sprite.LADDER_SPEED;
        }
    };

    void GroundedSpriteMovement::collideStopYTop( Sprite& sprite, int overlap )
    {
        sprite.acceleration_y_ = 0;
        sprite.hit_box_.y += sprite.vy_;
        sprite.vy_ = 0;
        sprite.hit_box_.y += overlap;
        sprite.jump_end_ = true;
    };

    void GroundedSpriteMovement::collideStopAny( Sprite& sprite, Collision& collision )
    {
        if ( !sprite.collide_bottom_prev_ || !sprite.collide_top_prev_ )
        {
            if ( collision.collideLeft() )
            {
                collideStopXLeft( sprite, collision.overlapXLeft() );
                sprite.collide_left_ = true;
            }

            if ( collision.collideRight() )
            {
                collideStopXRight( sprite, collision.overlapXRight() );
                sprite.collide_right_ = true;
            }
        }

        if ( collision.collideBottom() )
        {
            collideStopYBottom( sprite, collision.overlapYBottom() );
            sprite.collide_top_ = true;
        }

        if ( collision.collideTop() )
        {
            if ( !sprite.collide_top_prev_ )
            {
                collideStopYTop( sprite, collision.overlapYTop() );
            }

            sprite.collide_bottom_ = true;
        }
    };

    bool GroundedSpriteMovement::onGroundPadding( Sprite& sprite ) const
    {
        return sprite.on_ground_ || ( sprite.on_ground_padding_.on() && !sprite.on_ground_padding_.done() );
    };

    const Collision GroundedSpriteMovement::testCollision( const Sprite& me, const Object& them ) const
    {
        int overlap_x_left   = 0;
        int overlap_x_right  = 0;
        int overlap_y_top    = 0;
        int overlap_y_bottom = 0;
		
		// Keep character from catching on walls moving vertically.
        if
        (
            me.leftSubPixels() + 4000 < them.rightSubPixels() &&
            me.rightSubPixels() - 4000 > them.leftSubPixels() &&
            me.topSubPixels() < them.bottomSubPixels() &&
            me.bottomSubPixels() > them.topSubPixels()
        )
        {
            if ( me.centerYSubPixels() > them.centerYSubPixels() )
                overlap_y_top = them.bottomSubPixels() - me.topSubPixels();
            else
                overlap_y_bottom = me.bottomSubPixels() - them.topSubPixels();
        }

		// But allow character to stand on the tip-ends o' blocks.
        if
        (
            me.leftSubPixels() + 1000 < them.rightSubPixels() &&
            me.rightSubPixels() - 3000 > them.leftSubPixels() &&
            me.topSubPixels() < them.bottomSubPixels() &&
            me.bottomSubPixels() > them.topSubPixels()
        )
        {
            if ( me.centerYSubPixels() < them.centerYSubPixels() )
                overlap_y_bottom = me.bottomSubPixels() - them.topSubPixels();
        }

        if
        (
            me.leftSubPixels() < them.rightSubPixels() &&
            me.rightSubPixels() > them.leftSubPixels() &&
            me.topSubPixels() + 8000 < them.bottomSubPixels() &&
            me.bottomSubPixels() - 4000 > them.topSubPixels() // Keep character from getting caught on sides o' floor blocks.
        )
        {
            if ( me.centerXSubPixels() < them.centerXSubPixels() )
                overlap_x_right = them.rightSubPixels() - me.leftSubPixels();
            else if ( me.centerXSubPixels() > them.centerXSubPixels() )
                overlap_x_left = me.rightSubPixels() - them.leftSubPixels();
        }
		
        return Collision( overlap_x_left, overlap_x_right, overlap_y_top, overlap_y_bottom );
	};