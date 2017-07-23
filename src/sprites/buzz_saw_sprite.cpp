


// Name
//===================================
//
// BuzzSawSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "buzz_saw_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    BuzzSawSprite::BuzzSawSprite( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/buzzsaw.png", 0, 0, false, false, 0, true, -16, -16, 32, 32 ), x, y, 32, 32, { SpriteType::ENEMY }, 100, 1500, 3000, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_INSTANTLY_OFFSCREEN, false, false, true, false )
    {};

    BuzzSawSprite::~BuzzSawSprite() {};

    void BuzzSawSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( graphics_ != nullptr )
        {
            graphics_->rotation_ -= 5;
        }
    };

    void BuzzSawSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
        if ( their_collision.collideAny() )
        {
            them.killNoAnimation();
        }
    };

