


// Name
//===================================
//
// EggnonSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "eggnon_sprite.h"
    #include "sprite_graphics.h"
    #include "sprite_system.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================


    EggnonSprite::State::State( int time, Type type )
    :
        timer_ ( { time } ),
        type_ ( type )
    {};

    void EggnonSprite::State::run( EggnonSprite& e )
    {
        timer_.update();
        update( e );
    };

    bool EggnonSprite::State::done() const
    {
        return timer_.done();
    };

    EggnonSprite::State::Type EggnonSprite::State::type() const
    {
        return type_;
    };

    EggnonSprite::MovingState::MovingState()
    :
        State( mezun::randInt( 128, 16 ), Type::MOVING )
    {};

    void EggnonSprite::MovingState::update( EggnonSprite& e )
    {
        if ( e.directionX() == Direction::Horizontal::RIGHT )
        {
            e.moveRight();
        }
        else
        {
            e.moveLeft();
        }

        if ( e.collidedRight() )
        {
            e.direction_x_ = Direction::Horizontal::LEFT;
        }

        if ( e.collidedLeft() )
        {
            e.direction_x_ = Direction::Horizontal::RIGHT;
        }
    };

    void EggnonSprite::MovingState::init( EggnonSprite& e, SpriteSystem& sprites )
    {
        e.direction_x_ = Direction::randomHorizontal();
    };

    EggnonSprite::DroppingState::DroppingState()
    :
        State( 32, Type::DROPPING )
    {};

    void EggnonSprite::DroppingState::update( EggnonSprite& e )
    {
        e.fullStopX();
    };

    void EggnonSprite::DroppingState::init( EggnonSprite& e, SpriteSystem& sprites )
    {
        sprites.spawn( SpriteSystem::SpawnSprite::SPIKE_EGG, e.xPixels(), e.yPixels() + 8 );
    };

    EggnonSprite::EggnonSprite( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/eggnon.png", 0, 0, false, false, 0, true ), x, y, 16, 24, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 80, 500, 200, 500, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLUTTERING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true, true )
    {
        jump_lock_ = false;
    };

    EggnonSprite::~EggnonSprite() {};

    void EggnonSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        // INIT
        if ( !state_ )
        {
            state_.reset( new MovingState() );
            state_->init( *this, sprites );
        }

        // UPDATE
        state_->run( *this );


        // SHIFT
        if ( state_->done() )
        {
            switch( state_->type() )
            {
                case ( State::Type::MOVING ):
                    state_.reset( new DroppingState() );
                break;
                case ( State::Type::DROPPING ):
                    state_.reset( new MovingState() );
                break;
            }

            state_->init( *this, sprites );
        }

        graphics_->flip_x_ = ( direction_x_ == Direction::Horizontal::RIGHT );

        if ( hit_box_.y > original_hit_box_.y )
        {
            jump();
        }
    };

    void EggnonSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
    };
