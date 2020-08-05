#include "audio.hpp"
#include "bouncing_ball_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

BouncingBallSprite::BouncingBallSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bouncing-ball.png", 0, 0, false, false, 0.0, false, -1, -1, 2, 2 ), x, y, 30, 30, {}, 250, 4500, 250, 4500, Direction::Horizontal::__NULL, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

BouncingBallSprite::~BouncingBallSprite() {};

void BouncingBallSprite::customUpdate( LevelState& level_state )
{
    switch ( direction_y_ )
    {
        case ( Direction::Vertical::UP ):
        {
            moveUp();
        }
        break;
        case ( Direction::Vertical::DOWN ):
        {
            moveDown();
        }
        break;
    }

    if ( collide_top_ )
    {
        direction_y_ = Direction::Vertical::UP;
    }
    else if ( vy_ <= -jump_top_speed_ )
    {
        direction_y_ = Direction::Vertical::DOWN;
    }
};

void BouncingBallSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( their_collision.collideBottom() )
    {
        them.bounce();
        Audio::playSound( Audio::SoundType::BOUNCE );
    }
    else if ( their_collision.collideAny() )
    {
        level_state.health().hurt();
    }
};
