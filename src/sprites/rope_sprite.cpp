


// Name
//===================================
//
// RopeSprite
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "rope_sprite.hpp"
    #include "rope_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    RopeSprite::RopeSprite( int x, int y, int height, int distance, int speed )
    :
        Sprite( std::make_unique<RopeGraphics> (), x, y, 8, Unit::BlocksToPixels( height ), {}, speed, speed, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false, true, true ),
        on_ ( false ),
        action_ ( distance )
    {};

    RopeSprite::~RopeSprite() {};

    void RopeSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( on_ )
        {
            action_.update( *this, *graphics_ );
        }
    };

    void RopeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( their_collision.collideAny() )
            {
                them.touching_ladder_ = true;

                if ( them.onLadder() )
                {
                	on_ = true;
                    them.addToX( vx_ );
                }
            }
        }
    };

    void RopeSprite::reset()
    {
        hit_box_.x = original_hit_box_.x;
        on_ = false;
        vx_ = 0;
        acceleration_x_ = 0;
        action_.reset();
    };
