


// Name
//===================================
//
// SpriteMovement
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "sprite.h"
    #include "sprite_movement.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpriteMovement::SpriteMovement( Type type )
    :
        type_ ( type )
    {};

    SpriteMovement::Type SpriteMovement::type() const { return type_; };

    bool SpriteMovement::hasType( Type type ) const
    {
        return type == type_;
    };

    void SpriteMovement::moveLeft( Sprite& sprite )
    {
        sprite.acceleration_x_ = -sprite.start_speed_;
        sprite.direction_x_ = Direction::Horizontal::LEFT;
    };

    void SpriteMovement::moveRight( Sprite& sprite )
    {
        sprite.acceleration_x_ = sprite.start_speed_;
        sprite.direction_x_ = Direction::Horizontal::RIGHT;
    };

    void SpriteMovement::moveUp( Sprite& sprite )
    {
        sprite.acceleration_y_ = -sprite.start_speed_;
        sprite.direction_y_ = Direction::Vertical::UP;
    };

    void SpriteMovement::moveDown( Sprite& sprite )
    {
        sprite.acceleration_y_ = sprite.start_speed_;
        sprite.direction_y_ = Direction::Vertical::DOWN;
    };

    void SpriteMovement::jump( Sprite& sprite )
    {
    };

    void SpriteMovement::bounce( Sprite& sprite, int amount )
    {
    };

    void SpriteMovement::position( Sprite& sprite )
    {
        sprite.top_speed_upward_ = sprite.top_speed_;
        sprite.top_speed_downward_ = sprite.top_speed_;

        // If not moving anymo', start slowing down.
        if ( sprite.acceleration_x_ == 0 )
            sprite.vx_ /= sprite.traction_;
        if ( sprite.acceleration_y_ == 0 )
            sprite.vy_ /= sprite.traction_;
    };

    void SpriteMovement::collideStopXLeft( Sprite& sprite, int overlap )
    {
        if ( sprite.vx_ < 0 )
        {
        	//sprite.stopX();
            sprite.hit_box_.x -= sprite.vx_;
            sprite.vx_ = -( sprite.vx_ * sprite.bounce_ );
        }
    };

    void SpriteMovement::collideStopXRight( Sprite& sprite, int overlap )
    {
        if ( sprite.vx_ > 0 )
        {
        	//sprite.stopX();
            sprite.hit_box_.x -= sprite.vx_;
            sprite.vx_ = -( sprite.vx_ * sprite.bounce_ );
        }
    };

    void SpriteMovement::collideStopYBottom( Sprite& sprite, int overlap )
    {
        if ( sprite.vy_ > 0 )
        {
        	//sprite.stopY();
            sprite.hit_box_.y -= sprite.vy_;
            sprite.vy_ = -( sprite.vy_ * sprite.bounce_ );
        }
    };

    void SpriteMovement::collideStopYTop( Sprite& sprite, int overlap )
    {
        if ( sprite.vy_ < 0 )
        {
        	//sprite.stopY();
            sprite.hit_box_.y -= sprite.vy_;
            sprite.vy_ = -( sprite.vy_ * sprite.bounce_ );
        }
    };

    void SpriteMovement::collideStopAny( Sprite& sprite, Collision& collision )
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

        if ( collision.collideBottom() )
        {
            collideStopYBottom( sprite, collision.overlapYBottom() );
            sprite.collide_top_ = true;
        }

        if ( collision.collideTop() )
        {
            collideStopYTop( sprite, collision.overlapYTop() );
            sprite.collide_bottom_ = true;
        }
    };


    const Collision SpriteMovement::testCollision( const Sprite& me, const Object& them ) const
    {
        int overlap_x_left   = 0;
        int overlap_x_right  = 0;
        int overlap_y_top    = 0;
        int overlap_y_bottom = 0;
		
		if ( me.vx_ < 0 || 0 < me.vx_ )
		{
			if
			(
				me.leftSubPixels() + 4000 < them.rightSubPixels() &&
				me.rightSubPixels() - 4000  > them.leftSubPixels() &&
				me.topSubPixels() < them.bottomSubPixels() &&
				me.bottomSubPixels() > them.topSubPixels()
			)
			{
				if ( me.centerYSubPixels() > them.centerYSubPixels() )
					overlap_y_top = them.bottomSubPixels() - me.topSubPixels();
				else
					overlap_y_bottom = me.bottomSubPixels() - them.topSubPixels();
			}
		}
		else
		{
			if
			(
				me.leftSubPixels() < them.rightSubPixels() &&
				me.rightSubPixels() > them.leftSubPixels() &&
				me.topSubPixels() < them.bottomSubPixels() &&
				me.bottomSubPixels() > them.topSubPixels()
			)
			{
				if ( me.centerYSubPixels() > them.centerYSubPixels() )
					overlap_y_top = them.bottomSubPixels() - me.topSubPixels();
				else
					overlap_y_bottom = me.bottomSubPixels() - them.topSubPixels();
			}
		}
		
		if ( me.vy_ < 0 || 0 < me.vy_ )
		{
			if
			(
				me.leftSubPixels() < them.rightSubPixels() &&
				me.rightSubPixels() > them.leftSubPixels() &&
				me.topSubPixels() + 4000 < them.bottomSubPixels() &&
				me.bottomSubPixels() - 4000 > them.topSubPixels()
			)
			{	
				if ( me.centerXSubPixels() < them.centerXSubPixels() )
					overlap_x_right = them.rightSubPixels() - me.leftSubPixels();
				else if ( me.centerXSubPixels() > them.centerXSubPixels() )
					overlap_x_left = me.rightSubPixels() - them.leftSubPixels();
			}
		}
		else
		{
			if
			(
				me.leftSubPixels() < them.rightSubPixels() &&
				me.rightSubPixels() > them.leftSubPixels() &&
				me.topSubPixels() < them.bottomSubPixels() &&
				me.bottomSubPixels() > them.topSubPixels()
			)
			{	
				if ( me.centerXSubPixels() < them.centerXSubPixels() )
					overlap_x_right = them.rightSubPixels() - me.leftSubPixels();
				else if ( me.centerXSubPixels() > them.centerXSubPixels() )
					overlap_x_left = me.rightSubPixels() - them.leftSubPixels();
			}
		}
		
        return Collision( overlap_x_left, overlap_x_right, overlap_y_top, overlap_y_bottom );
	};