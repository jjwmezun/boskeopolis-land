





// Name
//===================================
//
// PlayerSpriteFluttering
//


// DEPENDENCIES
//===================================

    #include "camera.hpp"
    #include "input.hpp"
    #include "player_sprite_fluttering.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PlayerSpriteFluttering::PlayerSpriteFluttering( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/autumn_cart.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 28, 28, { SpriteType::HERO }, 160, 2000, 4000, 7000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLUTTERING, CameraMovement::PERMANENT, false )
    {
    };

    PlayerSpriteFluttering::~PlayerSpriteFluttering() {};

    void PlayerSpriteFluttering::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {

        if ( Input::held( Input::Action::RUN ) )
        {
            run();
        }
        else
        {
            stopRunning();
        }

        if ( Input::held( Input::Action::JUMP ) )
        {
            jump();
            jump_lock_ = true;
        }
        else
        {
            jump_lock_ = false;
        }

        if ( Input::held( Input::Action::MOVE_LEFT ) && !Input::held( Input::Action::MOVE_RIGHT ) )
        {
            moveLeft();
        }
        else if ( Input::held( Input::Action::MOVE_RIGHT ) && !Input::held( Input::Action::MOVE_LEFT ) )
        {
            moveRight();
        }
        else
        {
            stopX();
        }

        if ( Input::held( Input::Action::MOVE_UP ) )
        {
            jump_start_speed_ = 6000;
            jump_top_speed_ = 10000;
        }
        else
        {
            jump_start_speed_ = 4000;
            jump_top_speed_ = 7000;
        }

        if ( Input::held( Input::Action::MOVE_DOWN ) )
        {
            gravity_start_speed_ = GRAVITY_START_SPEED*1.5;
            gravity_top_speed_ = GRAVITY_TOP_SPEED*1.5;
        }
        else
        {
            gravity_start_speed_ = GRAVITY_START_SPEED;
            gravity_top_speed_ = GRAVITY_TOP_SPEED;
        }

        boundaries( camera, lvmap );
        camera.adjust( *this, lvmap );
    };

    void PlayerSpriteFluttering::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
    };
