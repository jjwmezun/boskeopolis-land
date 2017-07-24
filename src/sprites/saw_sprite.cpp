



// Name
//===================================
//
// SawSprite
//


// DEPENDENCIES
//===================================

    #include "camera.hpp"
    #include "collision.hpp"
    #include "saw_graphics.hpp"
    #include "saw_sprite.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SawSprite::SawSprite( int x, int y )
    :
        Sprite( std::make_unique<SawGraphics> (), x, y, 64, 256, {}, 1500, 1500, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, false, true, true )
    {};

    SawSprite::~SawSprite() {};

    void SawSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
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

    void SawSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
    {
        if ( their_collision.collideAny() && them.hasType( SpriteType::HERO ) )
        {
            them.killNoAnimation();
        }
    };
