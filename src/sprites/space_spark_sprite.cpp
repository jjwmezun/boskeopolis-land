#include "space_spark_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

SpaceSparkSprite::SpaceSparkSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/space-spark.png" ), x, y, 16, 16, { SpriteType::ENEMY }, 500, 4000, 0, 0, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
    left_edge_ ( ( direction == Direction::Horizontal::RIGHT ) ? original_hit_box_.x : original_hit_box_.x - Unit::BlocksToSubPixels( 6 ) ),
    right_edge_ ( ( direction == Direction::Horizontal::RIGHT ) ? original_hit_box_.x + Unit::BlocksToSubPixels( 7 ) : original_hit_box_.x + Unit::BlocksToSubPixels( 1 ) )
{};

SpaceSparkSprite::~SpaceSparkSprite() {};

void SpaceSparkSprite::customUpdate( LevelState& level_state )
{
    graphics_->rotation_ += 5.0;
    switch ( direction_x_ )
    {
        case ( Direction::Horizontal::LEFT ):
        {
            moveLeft();
            if ( hit_box_.x <= left_edge_ )
            {
                direction_x_ = Direction::Horizontal::RIGHT;
            }
        }
        break;
        case ( Direction::Horizontal::RIGHT ):
        {
            moveRight();
            if ( rightSubPixels() >= right_edge_ )
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
