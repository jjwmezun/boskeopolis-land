






// Name
//===================================
//
// FallingBoughSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "falling_bough_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    FallingBoughSprite::FallingBoughSprite( int x, int y, Direction::Horizontal direction )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVTILESET_CITY, 128, 40 ) ), x, y, 24, 8, {}, 10, 1400, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false )
    {
        if ( direction == Direction::Horizontal::LEFT )
        {
            hit_box_.x -= 8000;
        }
        else
        {
            if ( graphics_ != nullptr )
                graphics_->flip_x_ = true;
        }
    };

    FallingBoughSprite::~FallingBoughSprite() {};

    void FallingBoughSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( falling_timer_.on() )
            falling_timer_.update();

        if ( falling_timer_.done() )
            movement_ = MOVEMENTS[ SpriteMovement::Type::GROUNDED ];
    };

    void FallingBoughSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        if ( them.hasType( Sprite::SpriteType::HERO ) )
        {
            if ( their_collision.collideBottom() && them.prevBottomSubPixels() <= topSubPixels() + 1000  )
            {
                them.collideStopYBottom( their_collision.overlapYBottom() );
                them.addToY( vy_ );

                if ( !falling_timer_.on() )
                    falling_timer_.start();
            }
        }
    };

    void FallingBoughSprite::reset()
    {
        hit_box_.y = original_hit_box_.y;
        movement_ = MOVEMENTS[ SpriteMovement::Type::FLOATING ];
        vy_ = 0;
        acceleration_y_ = 0;
        falling_timer_.stop();
    };
