#include "horizontal_spark_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

HorizontalSparkSprite::HorizontalSparkSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 250, 6000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

HorizontalSparkSprite::~HorizontalSparkSprite() {};

void HorizontalSparkSprite::customUpdate( LevelState& level_state )
{
    moveInDirectionX();
	turnOnCollide();
};

void HorizontalSparkSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
