#include "falling_star_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

FallingStarSprite::FallingStarSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/star.png", 0, 0, false, false, 0.0, true, -4, -4, 8, 8 ), x, y, 8, 8, { SpriteType::ENEMY }, 1000, 1000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, true, false )
{};

FallingStarSprite::~FallingStarSprite() {};

void FallingStarSprite::customUpdate( LevelState& level_state )
{
    moveDown();
    acceleration_x_ = -500;
    graphics_->rotation_ -= 1.0;
    if ( fellInBottomlessPit( level_state.currentMap() ) )
    {
        killNoAnimation();
    }
};

void FallingStarSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
