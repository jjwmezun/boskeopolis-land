#include "pollo_flame_sprite.hpp"
#include "sprite_graphics.hpp"

PolloFlameSprite::PolloFlameSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/pollo-picante.png", 128, 12, false, false, 0.0, -1, -1, 2, 2 ), x, y, 6, 2, { SpriteType::ENEMY }, 2000, 2000, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::DESPAWN_OFFSCREEN, false, false, true )
{};

PolloFlameSprite::~PolloFlameSprite() {};

void PolloFlameSprite::customUpdate( LevelState& level_state )
{
    moveInDirectionX();
};

void PolloFlameSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
