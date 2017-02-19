



// Name
//===================================
//
// SawSprite
//


// DEPENDENCIES
//===================================

    #include "camera.h"
    #include "collision.h"
    #include "saw_graphics.h"
    #include "saw_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SawSprite::SawSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SawGraphics() ), x, y, 64, 256, {}, 1500, 1500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, true, true )
    {};

    SawSprite::~SawSprite() {};

    void SawSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
        moveRight();

        if ( camera.x() < Unit::SubPixelsToPixels( hit_box_.x ) + 4 )
            camera.moveRight( Unit::SubPixelsToPixels( hit_box_.x ) + 4 - camera.x() );

        ++counter_;

        if ( counter_ % 16 == 1 )
            going_up_ = !going_up_;

        if ( going_up_ )
            hit_box_.y-=1000;
        else
            hit_box_.y+=1000;
    };

    void SawSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        if ( their_collision.collideAny() && them.hasType( SpriteType::HERO ) )
        {
            them.killNoAnimation();
        }
    };
