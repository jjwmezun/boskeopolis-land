
// Name
//===================================
//
// Sprite
//


// DEPENDENCIES
//===================================

    #include "block.h"
    #include "camera.h"
    #include "collision.h"
    #include "fluttering_sprite_movement.h"
    #include "inventory_level.h"
    #include "map.h"
    #include "sprite.h"
    #include "sprite_graphics.h"
    #include "swimming_sprite_movement.h"
    #include "input.h"
    #include "unit.h"


// STATIC PROPERTIES
//===================================

    std::map<SpriteMovement::Type, SpriteMovement*> Sprite::MOVEMENTS =
    {
        { SpriteMovement::Type::GROUNDED,   new GroundedSpriteMovement() },
        { SpriteMovement::Type::FLOATING,   new SpriteMovement() },
        { SpriteMovement::Type::FLUTTERING, new FlutteringSpriteMovement() },
        { SpriteMovement::Type::SWIMMING,   new SwimmingSpriteMovement() }
    };
    double Sprite::traction_ = TRACTION_NORMAL;
    int Sprite::resistance_x_ = RESISTANCE_X_NORMAL;


// METHODS
//===================================

    Sprite::Sprite
    (
        std::unique_ptr<SpriteGraphics> graphics,
        int x,
        int y,
        int width,
        int height,
		std::vector<SpriteType> type,
        int start_speed,
        int top_speed,
        int jump_start_speed,
        int jump_top_speed,
        Direction::Horizontal direction_x,
        Direction::Vertical direction_y,
        std::unique_ptr<SpriteComponent> component,
        SpriteMovement::Type physics_state,
        CameraMovement camera_movement,
        bool despawn_when_dead,
        bool block_interact,
        bool sprite_interact,
        bool impervious,
        double bounce,
        int max_hp,
        int hp
    )
    :
        Object( x, y, width, height ),
        graphics_ ( std::move( graphics ) ),
        types_ ( type ),
        jump_start_speed_ ( jump_start_speed ),
        jump_top_speed_normal_ ( jump_top_speed ),
        jump_top_speed_ ( jump_top_speed ),
        start_speed_walk_ ( start_speed ),
        top_speed_walk_ ( top_speed ),
        start_speed_ ( start_speed ),
        top_speed_ ( top_speed ),
        start_speed_run_ ( start_speed ),
        top_speed_run_ ( top_speed*2 ),
        direction_x_ ( direction_x ),
        direction_x_orig_ ( direction_x ),
        direction_y_ ( direction_y ),
        direction_y_orig_ ( direction_y ),
        component_ ( std::move( component ) ),
        status_ ( max_hp, hp ),
        movement_ ( MOVEMENTS.at( physics_state ) ),
        camera_movement_ ( camera_movement ),
        despawn_when_dead_ ( despawn_when_dead ),
        block_interact_ ( block_interact ),
        sprite_interact_ ( sprite_interact ),
        impervious_ ( impervious ),
        bounce_ ( bounce ),
        on_ground_padding_ ( { 4, false } ),
		direction_ ( Direction::Simple::__NULL )
    {};

    Sprite::~Sprite()
    {
    };

    bool Sprite::fellInBottomlessPit( Map& lvmap ) const
    {
        return topSubPixels() > Unit::PixelsToSubPixels( lvmap.heightPixels() );
    };

    void Sprite::update( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( !isDead() )
        {
            if ( component_ )
            {
                component_->update( *this, *graphics_ );
            }
            customUpdate( input, camera, lvmap, game, events, sprites, blocks );
            status_.update( *this, *graphics_ );
        }
        else
        {
            if ( !dead_no_animation_ )
            {
                deathAction( camera );
            }
            else
            {
                changeMovement( SpriteMovement::Type::FLOATING );
                acceleration_x_ = 0;
                acceleration_y_ = 0;
                vx_ = 0;
                vy_ = 0;

                if ( death_timer_.done() )
                {
                    death_finished_ = true;
                }

                if ( !death_timer_.on() )
                {
                    death_timer_.start();
                }
                else
                {
                    death_timer_.update();
                }
            }
        }

        position();

        if ( interactsWithBlocks() && movement_->type() == SpriteMovement::Type::GROUNDED && graphics_ )
        {
            switch ( on_slope_ )
            {
                case ( Direction::Horizontal::LEFT ):
                    graphics_->rotation_ = -30;
                break;
                case ( Direction::Horizontal::RIGHT ):
                    graphics_->rotation_ = 30;
                break;
                case ( Direction::Horizontal::__NULL ):
                    graphics_->rotation_ = 0;
                break;
            }
        }
		
		graphics_->update( *this );

        in_water_prev_ = in_water_;
        in_water_ = false;
        on_slope_ = Direction::Horizontal::__NULL;
        is_sliding_prev_ = is_sliding_;
        is_sliding_ = false;
    };

    void Sprite::render( Graphics& graphics, Camera& camera, bool priority )
    {
        if ( camera.onscreen( hitBox() ) && graphics_ != nullptr )
        {
            graphics_->render( graphics, Unit::SubPixelsToPixels( hit_box_ ), &camera, priority );
        }
    };

    void Sprite::moveLeft()
    {
        if ( movement_ != nullptr )
        {
            movement_->moveLeft( *this );
        }
    };

    void Sprite::moveRight()
    {
        if ( movement_ != nullptr )
        {
            movement_->moveRight( *this );
        }
    };

    void Sprite::stopX()
    {
        acceleration_x_ = 0;
    };

    void Sprite::stopY()
    {
        acceleration_y_ = 0;
    };

    void Sprite::fullStopX()
    {
        acceleration_x_ = 0;
        vx_ = 0;
    };

    void Sprite::fullStopY()
    {
        acceleration_y_ = 0;
        vy_ = 0;
    };

    void Sprite::stopDucking()
    {
        acceleration_x_ = 0;
    };

    void Sprite::positionX()
    {
        vx_ += acceleration_x_;

        if ( vx_ > top_speed_ )
            vx_ = top_speed_;
        if ( vx_ < -top_speed_ )
            vx_ = -top_speed_;

        vx_ += resistance_x_;

        hit_box_.x += vx_;
    };

    void Sprite::positionY()
    {
        vy_ += acceleration_y_;

        if ( vy_ > top_speed_downward_ )
        {
            vy_ = top_speed_downward_;
        }

        if ( vy_ < -top_speed_upward_ )
        {
            vy_ = -top_speed_upward_;
        }

        hit_box_.y += vy_;
    };

    void Sprite::position()
    {
        x_prev_ = hit_box_.x;
        y_prev_ = hit_box_.y;

        if ( movement_ != nullptr )
            movement_->position( *this );

        positionX();
        positionY();

        if ( acceleration_x_ != 0 || acceleration_y_ != 0 )
            is_moving_ = true;
        else
            is_moving_ = false;

        collide_top_prev_ = collide_top_;
        collide_bottom_prev_ = collide_bottom_;
        collide_top_ = false;
        collide_bottom_ = false;
        collide_left_ = false;
        collide_right_ = false;
    };

    void Sprite::boundaries( Camera& camera, Map& lvmap )
    {
        if ( hit_box_.x < Unit::PixelsToSubPixels( camera.x() ) )
        {
            collideStopXLeft( abs ( camera.x() - hit_box_.x ) );
        }

        if ( hit_box_.x + hit_box_.w > Unit::PixelsToSubPixels( camera.right() ) )
        {
            collideStopXRight( ( hit_box_.x + hit_box_.w ) - Unit::PixelsToSubPixels( camera.right() ) );
        }

        if ( hit_box_.y < -hit_box_.h )
        {
            hit_box_.y = -hit_box_.h;
        }

/*
        if ( lvmap.scrollLoop() )
        {
            if ( hit_box_.x < 0 )
            {
                camera.setPosition( camera.x() + lvmap.widthPixels(), camera.y() );
                hit_box_.x += Unit::PixelsToSubPixels( lvmap.widthPixels() );
            }
            else if ( hit_box_.x > Unit::PixelsToSubPixels( lvmap.widthPixels() ) )
            {
                hit_box_.x = hit_box_.x - Unit::PixelsToSubPixels( lvmap.widthPixels() );
                camera.setPosition( camera.x() - lvmap.widthPixels(), camera.y() );
            }
        }*/
    };

    void Sprite::collideStopXLeft( int overlap )
    {
        if ( movement_ != nullptr )
        {
            movement_->collideStopXLeft( *this, overlap );
        }
    };

    void Sprite::collideStopXRight( int overlap )
    {
        if ( movement_ != nullptr )
        {
            movement_->collideStopXRight( *this, overlap );
        }
    };

    void Sprite::collideStopYBottom( int overlap )
    {
        if ( movement_ != nullptr )
        {
            movement_->collideStopYBottom( *this, overlap );
        }
    };

    void Sprite::collideStopYTop( int overlap )
    {
        if ( movement_ != nullptr )
        {
            movement_->collideStopYTop( *this, overlap );
        }
    };

    bool Sprite::onGround() const
    {
        return on_ground_;
    };

    bool Sprite::onGroundPadding() const
    {
        return on_ground_;
    };

    bool Sprite::isJumping() const
    {
        return is_jumping_;
    };

    void Sprite::jump()
    {
        if ( movement_ != nullptr )
            movement_->jump( *this );
    };

    void Sprite::bounce( int amount )
    {
        if ( movement_ != nullptr )
            movement_->bounce( *this, amount );
    };

    void Sprite::slowFall()
    {
        gravity_start_speed_ = GRAVITY_START_SPEED * .75;
        gravity_top_speed_ = GRAVITY_TOP_SPEED * .85;
    };

    void Sprite::fastFall()
    {
        gravity_start_speed_ = GRAVITY_START_SPEED;
        gravity_top_speed_ = GRAVITY_TOP_SPEED;
    };

    bool Sprite::isRunning() const
    {
        return is_running_;
    };

    void Sprite::run()
    {
        is_running_ = true;
        top_speed_ = top_speed_run_;
        start_speed_ = start_speed_run_;
    };

    void Sprite::stopRunning()
    {
        is_running_ = false;
        top_speed_ = top_speed_walk_;
        start_speed_ = start_speed_walk_;
    };

    void Sprite::interact( Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        Collision my_collision = testCollision( them );
        Collision their_collision = them.testCollision( *this );

        customInteract( my_collision, their_collision, them, blocks, sprites );
    };

    bool Sprite::canJump() const
    {
        return can_jump_;
    };

    bool Sprite::isDucking() const
    {
        return is_ducking_;
    };

    Direction::Horizontal Sprite::directionX() const
    {
        return direction_x_;
    };

    Direction::Vertical Sprite::directionY() const
    {
        return direction_y_;
    };

    bool Sprite::onGroundPrev() const
    {
        return on_ground_prev_;
    };

    bool Sprite::isMoving() const
    {
        return is_moving_;
    };

    bool Sprite::onLadder() const
    {
        return on_ladder_;
    };

    bool Sprite::onLadderPrev() const
    {
        return on_ladder_prev_;
    };

    bool Sprite::touchingLadder() const
    {
        return touching_ladder_;
    };

    bool Sprite::touchingLadderPrev() const
    {
        return touching_ladder_prev_;
    };

    void Sprite::touchLadder()
    {
        touching_ladder_ = true;
    };

    void Sprite::grabLadder()
    {
        if ( !isDucking() )
            on_ladder_ = true;
    };

    void Sprite::releaseLadder()
    {
        on_ladder_ = false;
    };

    void Sprite::moveUp()
    {
        if ( movement_ != nullptr )
        {
            movement_->moveUp( *this );
        }
    };

    void Sprite::moveDown()
    {
        if ( movement_ != nullptr )
        {
            movement_->moveDown( *this );
        }
    };

    void Sprite::lookUp()
    {
        looking_up_ = true;
    };

    bool Sprite::lookingUp() const
    {
        return looking_up_;
    };

    void Sprite::stopLookingUp()
    {
        looking_up_ = false;
    };

    void Sprite::kill()
    {
        status_.kill();
    };

    bool Sprite::isDead() const
    {
        return status_.isDead();
    };

    void Sprite::setPosition( int x, int y )
    {
        hit_box_.x = Unit::PixelsToSubPixels( x );
        hit_box_.y = Unit::PixelsToSubPixels( y );
    };

    bool Sprite::hasType( Sprite::SpriteType type ) const
    {
		for ( auto& t : types_ )
		{
			if ( type == t ) return true;
		}
		
		return false;
    };

    void Sprite::hurt( int amount )
    {
        if ( !impervious_ )
        {
            status_.hurt( amount );
        }
    };

    void Sprite::heal( int amount )
    {
        status_.heal( amount );
    };

    int Sprite::hp() const
    {
        return status_.hp();
    };


    void Sprite::collideStopAny( Collision& collision )
    {
        if ( movement_ != nullptr )
        {
            movement_->collideStopAny( *this, collision );
        }
    };

    bool Sprite::interactsWithBlocks() const
    {
        return block_interact_;
    };

    bool Sprite::interactsWithSprites() const
    {
        return sprite_interact_;
    };

    bool Sprite::collidedRight() const
    {
        return collide_right_;
    };

    bool Sprite::collidedLeft() const
    {
        return collide_left_;
    };

    void Sprite::reset()
    {
        resetPosition();
    };

    void Sprite::resetPosition()
    {
        hit_box_.x = original_hit_box_.x;
        hit_box_.y = original_hit_box_.y;
    };

    void Sprite::slideLeft()
    {
        is_sliding_ = true;
        moveLeft();
    };

    void Sprite::slideRight()
    {
        is_sliding_ = true;
        moveRight();
    };

    bool Sprite::collideTopOnly( Collision& collision, Object& them ) const
    {
        return collision.collideTop() && prevBottomSubPixels() >= them.ySubPixels() - 1000;
    };

    bool Sprite::hasCameraMovement( CameraMovement type ) const
    {
        return type == camera_movement_;
    };

    Sprite::CameraMovement Sprite::cameraMovement() const
    {
        return camera_movement_;
    };

    bool Sprite::despawnWhenDead() const
    {
        return despawn_when_dead_;
    }

    void Sprite::deathAction( Camera& camera )
    {
        defaultDeathAction( camera );
    };

    void Sprite::defaultDeathAction( Camera& camera )
    {
        acceleration_x_ = 0;
        vx_ = 0;
        block_interact_ = false;
        sprite_interact_ = false;
        movement_ = MOVEMENTS[ SpriteMovement::Type::GROUNDED ];
        on_ladder_ = false;

        if ( camera.offscreen( hit_box_ ) )
        {
            death_finished_ = true;
        }
    };

    bool Sprite::deathFinished() const
    {
        return death_finished_;
    };

    void Sprite::changeMovement( SpriteMovement::Type type )
    {
        movement_ = MOVEMENTS[ type ];
    };

    void Sprite::killNoAnimation()
    {
        kill();
        dead_no_animation_ = true;
    };

    const SpriteStatus& Sprite::status() const
    {
        return status_;
    };

    bool Sprite::collideBottomOnly( const Collision& collision, const Object& other ) const
    {
        return collision.collideBottom() && prevBottomSubPixels() <= other.ySubPixels() + 1000;
    };

    void Sprite::swim()
    {
        in_water_ = true;
        changeMovement( SpriteMovement::Type::SWIMMING );
    };

    void Sprite::bounceLeft( int overlap )
    {
        stopX();

        if ( vx_ < 0 )
        {
            hit_box_.x -= vx_;
            vx_ = -( vx_ * ( bounce_ * 3 ) );
        }
    };

    void Sprite::bounceRight( int overlap )
    {
        stopX();

        if ( vx_ > 0 )
        {
            hit_box_.x -= vx_;
            vx_ = -( vx_ * ( bounce_ * 3 ) );
        }
    };

    void Sprite::bounceDownward( int overlap )
    {
        stopY();

        if ( vy_ < 0 )
        {
            hit_box_.y -= vy_;
            vy_ = -( vy_ * ( bounce_ * 3 ) );
        }
    };

    bool Sprite::isSlidingPrev() const
    {
        return is_sliding_prev_;
    };

	const Collision Sprite::testCollision( const Object& them ) const
	{
        if ( movement_ != nullptr )
        {
            return movement_->testCollision( *this, them );
        }	
	};