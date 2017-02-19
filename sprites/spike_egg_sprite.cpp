



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
        Sprite( std::unique_ptr<SpriteGraphics> ( new SpriteGraphics( Graphics::SpriteSheet::LVSPRITES_EGG, 0, 0, false, false, 0, false, -2, -2, 4, 4 ) ), x, y, 12, 12, { SpriteType::ENEMY }, 1000, 3000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_INSTANTLY_OFFSCREEN, true, false )
    {};

    SpikeEggSprite::~SpikeEggSprite() {};

    void SpikeEggSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
        graphics_->rotation_ -= 5;
    };

    void SpikeEggSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
    };

    void SpikeEggSprite::reset()
    {
        death_finished_ = true;
    };
