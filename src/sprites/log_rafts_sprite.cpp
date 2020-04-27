#include "log_rafts_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

#include <cstdio>

LogRaftsSprite::LogRaftsSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/box.png" ), 0, y, 72, 16, {}, 2000, 2000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT )
{
    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        rafts_[ i ] = { 0, hit_box_.y, hit_box_.w, hit_box_.h };
    }
};

LogRaftsSprite::~LogRaftsSprite() {};

void LogRaftsSprite::customUpdate( LevelState& level_state )
{
    moveLeft();
    if ( hit_box_.x < -Unit::BlocksToSubPixels( 8 ) )
    {
        hit_box_.x = 0;
    }

    const Camera& camera = level_state.camera();
    int x = hit_box_.x;
    while ( x < Unit::PixelsToSubPixels( camera.x() ) - Unit::BlocksToSubPixels( 8 ) )
    {
        x += Unit::BlocksToSubPixels( 8 );
    }

    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        if ( x <= Unit::BlocksToSubPixels( 35 ) )
        {
            rafts_[ i ].y = hit_box_.y + ( Unit::BlocksToSubPixels( 35 ) - x );
        }
        else if ( x < Unit::BlocksToSubPixels( 72 ) )
        {
            rafts_[ i ].y = hit_box_.y;
        }
        else if ( x <= Unit::BlocksToSubPixels( 75 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 3 ) + ( Unit::BlocksToSubPixels( 75 ) - x );
        }
        else if ( x < Unit::BlocksToSubPixels( 148 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 3 );
        }
        else if ( x <= Unit::BlocksToSubPixels( 151 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 6 ) + ( Unit::BlocksToSubPixels( 151 ) - x );
        }
        else if ( x < Unit::BlocksToSubPixels( 157 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 6 );
        }
        else if ( x <= Unit::BlocksToSubPixels( 160 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 9 ) + ( Unit::BlocksToSubPixels( 160 ) - x );
        }
        else if ( x < Unit::BlocksToSubPixels( 166 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 9 );
        }
        else if ( x <= Unit::BlocksToSubPixels( 169 ) )
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 12 ) + ( Unit::BlocksToSubPixels( 169 ) - x );
        }
        else
        {
            rafts_[ i ].y = hit_box_.y - Unit::BlocksToSubPixels( 12 );
        }

        rafts_[ i ].x = x;
        x += Unit::BlocksToSubPixels( 8 );
    }
};

void LogRaftsSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
        const bool blocks_in_the_way = !level_state.blocks().blocksInTheWay
        (
            {
                them.rightSubPixels(),
                them.topSubPixels(),
                1000,
                them.heightSubPixels()
            },
            BlockComponent::Type::SOLID
        );

        for ( int i = 0; i < MAX_RAFTS; ++i )
        {
		    const Collision collision = movement_->testCollision( them, rafts_[ i ] );
            if ( collision.collideAny() )
            {
                if ( collision.collideBottom() )
                {
                    them.hit_box_.x += vx_;
                }
                them.collideStopAny( collision );
            }
        }
	}
};

void LogRaftsSprite::render( Camera& camera, bool priority )
{

    for ( int i = 0; i < MAX_RAFTS; ++i )
    {
        graphics_->render( Unit::SubPixelsToPixels( rafts_[ i ] ), &camera, priority );
    }
};
