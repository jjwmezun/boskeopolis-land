



// Name
//===================================
//
// SpikeEggSprite
//


// DEPENDENCIES
//===================================

    #include "collision.hpp"
    #include "spike_egg_sprite.hpp"
    #include "sprite_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpikeEggSprite::SpikeEggSprite( int x, int y )
    :
        Sprite( std::make_unique<SpriteGraphics> ( "sprites/egg.png", 0, 0, false, false, 0, -2, -2, 4, 4 ), x, y, 12, 12, { SpriteType::ENEMY }, 1000, 3000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_INSTANTLY_OFFSCREEN, true, false )
    {};

    SpikeEggSprite::~SpikeEggSprite() {};

    void SpikeEggSprite::customUpdate( LevelState& level_state )
    {
        graphics_->rotation_ -= 5;
    };

    void SpikeEggSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
    {
    };

    void SpikeEggSprite::reset()
    {
        death_finished_ = true;
    };
