#include "audio.hpp"
#include "bouncing_ball_sprite.hpp"
#include "collision.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "render.hpp"
#include "sprite_graphics.hpp"

BouncingBallSprite::BouncingBallSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/bouncing-ball.png", 0, 0, false, false, 0.0, false, -3, -3, 6, 6 ), x, y, 26, 26, {}, 250, 4500, 250, 4500, Direction::Horizontal::__NULL, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

BouncingBallSprite::~BouncingBallSprite() {};

void BouncingBallSprite::customUpdate( LevelState& level_state )
{
    switch ( direction_y_ )
    {
        case ( Direction::Vertical::UP ):
        {
            if ( hit_box_.h < 26000 )
            {
                hit_box_.h += 8000;
                if ( hit_box_.h > 26000 )
                {
                    hit_box_.h = 26000;
                }
            }
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
        if ( hit_box_.h <= 16000 )
        {
            hit_box_.h = 16000;
            direction_y_ = Direction::Vertical::UP;
        }
        else
        {
            hit_box_.h -= 8000;
        }
    }
    else if ( vy_ <= -jump_top_speed_ )
    {
        direction_y_ = Direction::Vertical::DOWN;
    }
};

void BouncingBallSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
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
    }
};

void BouncingBallSprite::render( Camera& camera, bool priority )
{
    Render::renderObject( "sprites/bouncing-ball.png", { 0, 0, 32, 32 }, camera.relativeRect({ xPixels() - 3, yPixels() - 3, widthPixels() + 6, heightPixels() + 6 }) );
};
