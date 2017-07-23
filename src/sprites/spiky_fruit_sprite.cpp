






// Name
//===================================
//
// SpikyFruitSprite
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "spiky_fruit_sprite.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpikyFruitSprite::SpikyFruitSprite( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/spikyfruit.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 10, 1400, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_INSTANTLY_OFFSCREEN, false, false )
    {};

    SpikyFruitSprite::~SpikyFruitSprite() {};

    void SpikyFruitSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( movement_->hasType( SpriteMovement::Type::GROUNDED ) )
            if ( graphics_ != nullptr )
                graphics_->rotation_+=5;
    };

    void SpikyFruitSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( them.rightSubPixels() > leftSubPixels() - 16000 && them.leftSubPixels() < rightSubPixels() + 16000 )
                changeMovement( SpriteMovement::Type::GROUNDED );
        }
    };

    void SpikyFruitSprite::reset()
    {
        resetPosition();
        changeMovement( SpriteMovement::Type::FLOATING );
        vy_ = 0;
        acceleration_y_ = 0;

        if ( graphics_ != nullptr )
            graphics_->rotation_ = 0;
    };
