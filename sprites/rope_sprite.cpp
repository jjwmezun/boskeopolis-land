


// Name
//===================================
//
// RopeSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "rope_sprite.h"
    #include "rope_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    RopeSprite::RopeSprite( int x, int y, int height, int distance, int speed )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new RopeGraphics() ), x, y, 8, Unit::BlocksToPixels( height ), SpriteType::NOTYPE, speed, speed, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false, true, true ),
        on_ ( false ),
        action_ ( distance )
    {};

    RopeSprite::~RopeSprite() {};

    void RopeSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
        if ( on_ )
        {
            action_.update( *this, *graphics_ );
        }
    };

    void RopeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( their_collision.collideAny() )
            {
                them.touching_ladder_ = true;
                on_ = true;

                if ( them.onLadder() )
                {
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
