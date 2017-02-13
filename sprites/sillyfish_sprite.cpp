




// Name
//===================================
//
// SillyfishSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "sillyfish_sprite.h"
    #include "sillyfish_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SillyfishSprite::SillyfishSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SillyfishGraphics() ), x, y, 16, 32, SpriteType::NOTYPE, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
        start_ground_lock_ ( false ),
        switch_lock_ ( { 4, false } )
    {};

    SillyfishSprite::~SillyfishSprite() {};

    void SillyfishSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
        if ( directionX() == Direction::Horizontal::RIGHT )
        {
            moveRight();
        }
        else
        {
            moveLeft();
        }

        if ( !onGround() && !onGroundPrev() )
        {
            if ( start_ground_lock_ )
            {
                on_ground_ = true;
                fullStopY();

                if ( !switch_lock_.on() )
                {
                    stopX();
                    direction_x_ = Direction::switchHorizontal( direction_x_ );
                    switch_lock_.start();
                }
            }
        }

        if ( onGround() )
        {
            start_ground_lock_ = true;
        }

        if ( switch_lock_.done() )
        {
            switch_lock_.stop();
        }
        else if ( switch_lock_.on() )
        {
            switch_lock_.update();
        }
    };

    void SillyfishSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( them.collideBottom( their_collision, this ) )
            {
                kill();
                them.bounce();
            }
            else if ( their_collision.collideAny() && them.is_sliding_ )
            {
                kill();
            }
            else if ( their_collision.collideAny() && !isDead() )
            {
                them.hurt();
            }
        }
    };
