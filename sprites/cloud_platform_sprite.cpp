



// Name
//===================================
//
// CloudPlatformSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "cloud_platform_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    CloudPlatformSprite::CloudPlatformSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVSPRITES_CLOUD_PLATFORM, 0, 0, false, false, 0, true, 0, -6, 0, 10 ) ), x, y, 64, 8, {}, 800, 1200, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false ),
        started_ ( false )
    {};

    CloudPlatformSprite::~CloudPlatformSprite() {};

    void CloudPlatformSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( started_ )
        {
            moveRight();
        }
    };

    void CloudPlatformSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        if ( their_collision.collideBottom() )
        {
            them.collideStopYBottom( their_collision.overlapYBottom() );
            them.addToX( vx_ );
            them.addToY( vy_ );
            started_ = true;
        }
    };
