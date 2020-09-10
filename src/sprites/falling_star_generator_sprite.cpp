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
    start_ ( false ),
    prev_x_ ({ -999999, -9999999, -99999999 })
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
        if ( timer_ >= 30 )
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

//
// Spawns stars on current screen & screens on either side o’ current screen.
//
// To try decreasing the chances o’ stars clustering together, keep generating
// x positions for each spawn until each is far ’nough from previous x for
// each spawn.
//
void FallingStarGeneratorSprite::spawn( LevelState& level_state )
{
    const int y = level_state.camera().y() - 16;
    int x = 0;
    do
    {
        x = level_state.camera().x() + mezun::randInt( 0, -Unit::WINDOW_WIDTH_PIXELS );
    } while ( x < prev_x_[ 0 ] + 32 && x > prev_x_[ 0 ] - 32 );
    prev_x_[ 0 ] = x;
    level_state.sprites().spawn( std::unique_ptr<FallingStarSprite> ( new FallingStarSprite( x, y ) ) );
    do
    {
        x = level_state.camera().x() + mezun::randInt( Unit::WINDOW_WIDTH_PIXELS, 0 );
    } while ( x < prev_x_[ 1 ] + 32 && x > prev_x_[ 1 ] - 32 );
    prev_x_[ 1 ] = x;
    level_state.sprites().spawn( std::unique_ptr<FallingStarSprite> ( new FallingStarSprite( x, y ) ) );
    do
    {
        x = level_state.camera().x() + mezun::randInt( Unit::WINDOW_WIDTH_PIXELS * 2, Unit::WINDOW_WIDTH_PIXELS );
    } while ( x < prev_x_[ 2 ] + 32 && x > prev_x_[ 2 ] - 32 );
    prev_x_[ 2 ] = x;
    level_state.sprites().spawn( std::unique_ptr<FallingStarSprite> ( new FallingStarSprite( x, y ) ) );
}

void FallingStarGeneratorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
};
