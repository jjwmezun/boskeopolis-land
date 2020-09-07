#include "camera.hpp"
#include "collision.hpp"
#include "falling_star_sprite.hpp"
#include "falling_star_generator_sprite.hpp"
#include "level_state.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"
#include "unit.hpp"

#include <cstdio>

FallingStarGeneratorSprite::FallingStarGeneratorSprite( int x, int y )
:
	Sprite( nullptr, x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT ),
    timer_ ( 0 ),
    start_ ( false )
{};

FallingStarGeneratorSprite::~FallingStarGeneratorSprite() {};

void FallingStarGeneratorSprite::customUpdate( LevelState& level_state )
{
    if ( start_ )
    {
        for ( int i = 0; i < 20; ++i )
        {
            spawn( level_state );
        }
        start_ = false;
    }
    else
    {
        if ( timer_ >= 15 )
        {
            timer_ = 0;
            spawn( level_state );
        }
        else
        {
            ++timer_;
        }
    }
};

void FallingStarGeneratorSprite::spawn( LevelState& level_state ) const
{
    const int x = level_state.camera().x() + mezun::randInt( Unit::WINDOW_WIDTH_PIXELS * 2, 0 );
    const int y = level_state.camera().y() - 16;
    level_state.sprites().spawn( std::unique_ptr<FallingStarSprite> ( new FallingStarSprite( x, y ) ) );
}

void FallingStarGeneratorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
