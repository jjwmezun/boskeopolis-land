






// Name
//===================================
//
// SwimmingSpriteMovement
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "sprite.hpp"
    #include "swimming_sprite_movement.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SwimmingSpriteMovement::SwimmingSpriteMovement()
    :
        GroundedSpriteMovement( Type::SWIMMING )
    {};

    void SwimmingSpriteMovement::jump( Sprite& sprite )
    {
        if ( !sprite.jump_lock_ && !sprite.isDucking() )
        {
            sprite.acceleration_y_ = -SWIM_START_SPEED;
            sprite.is_jumping_ = true;
        }
    };

    void SwimmingSpriteMovement::position( Sprite& sprite )
    {

        if ( !sprite.onGround() && !sprite.isJumping() )
        {
            sprite.acceleration_y_ = sprite.gravity_start_speed_ / 2;
        }

        sprite.top_speed_upward_ = sprite.jump_top_speed_ / 2;
        sprite.top_speed_downward_ = sprite.gravity_top_speed_ / 2;

        sprite.on_ground_prev_ = sprite.on_ground_;
        sprite.on_ground_ = false;
		sprite.is_jumping_prev_ = sprite.is_jumping_;
        sprite.is_jumping_ = false;

        // If not moving anymo', start slowing down.
        if ( sprite.acceleration_x_ == 0 )
            sprite.vx_ /= sprite.traction_;
    };
