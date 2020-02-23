


// Name
//===================================
//
// BuzzSawSprite
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "buzz_saw_sprite.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BuzzSawSprite::BuzzSawSprite( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/buzzsaw.png", 0, 0, false, false, 0, true, -16, -16, 32, 32 ), x, y, 32, 32, { SpriteType::ENEMY }, 100, 1500, 3000, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_INSTANTLY_OFFSCREEN, false, false, true, false )
    {};

    BuzzSawSprite::~BuzzSawSprite() {};

    void BuzzSawSprite::customUpdate( LevelState& level_state )
    {
        if ( graphics_ != nullptr )
        {
            graphics_->rotation_ -= 5;
        }
    };

    void BuzzSawSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
    {
        if ( their_collision.collideAny() )
        {
            them.killNoAnimation();
        }
    };

