#include "space_spark_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

SpaceSparkSprite::SpaceSparkSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/space-spark.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 500, 4000, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

SpaceSparkSprite::~SpaceSparkSprite() {};

void SpaceSparkSprite::customUpdate( LevelState& level_state )
{
    switch ( direction_x_ )
    {
        case ( Direction::Horizontal::LEFT ):
        {
            moveLeft();
            if ( hit_box_.x <= original_hit_box_.x )
            {
                direction_x_ = Direction::Horizontal::RIGHT;
            }
        }
        break;
        case ( Direction::Horizontal::RIGHT ):
        {
            moveRight();
            if ( rightSubPixels() >= original_hit_box_.x + Unit::BlocksToSubPixels( 7 ) )
            {
                direction_x_ = Direction::Horizontal::LEFT;
            }
        }
        break;
    }
};

void SpaceSparkSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
