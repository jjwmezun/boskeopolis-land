



// Name
//===================================
//
// SpikeEggSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "spike_egg_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpikeEggSprite::SpikeEggSprite( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/egg.png", 0, 0, false, false, 0, false, -2, -2, 4, 4 ), x, y, 12, 12, { SpriteType::ENEMY }, 1000, 3000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_INSTANTLY_OFFSCREEN, true, false )
    {};

    SpikeEggSprite::~SpikeEggSprite() {};

    void SpikeEggSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        graphics_->rotation_ -= 5;
    };

    void SpikeEggSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
    };

    void SpikeEggSprite::reset()
    {
        death_finished_ = true;
    };
